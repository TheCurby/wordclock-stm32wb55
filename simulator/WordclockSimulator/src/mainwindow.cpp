#include "mainwindow.h"

bool bLED = false;
uint32_t u32Timer = 0;
uint16_t u16Seconds = 0;
s_RTC oRTC_l;

QString MainWindow::sGerman = "ESKISTLAMPMZEHNZWANZIGFÜNFVIERTELTGNACHVORJMHALBDZWÖLFPZWEINSIEBENKDREIJLFÜNFELFNEUNVIERWACHTZEHNRSNSECHS℃MUHR....";
QString MainWindow::sDutch = "HETKISAVIJFTIENATZAMPMOVERMEKWARTVOORSPMOVERHALFTHGÉÉNSTWEEAMCDRIEVIERVIJFZESZEVENONEGENACHTTIENELFTWAALF℃MUUR....";
QString MainWindow::sEnglish = "ITLISASAMPMACQUARTERDCTWENTYFIVEXHALFSTENFTOPASTERUNINEONESIXTHREEFOURFIVETWOEIGHTELEVENSEVENTWELVETEN℃EOCLOCK....";
QString MainWindow::sItalian = "SONORLEAMPMÈRLUNASDUEZTREOTTONOVEDIECIUNDICIDODICISETTEQUATTROCSEICINQUEAMENOEKUNLQUARTOVENTICINQUEDIECI℃MEZZA....";

MainWindow::MainWindow(){
    for(uint8_t i = 0; i < 114; i++){
        label[i].setAutoFillBackground(true);
        label[i].setAlignment(Qt::AlignCenter);
        label[i].setFixedSize(65,65);
        //label[i].setFixedWidth(65);
        //label[i].resize(65, 65);
        label[i].setStyleSheet("font-family:Source Code Pro;font-size:40px;margin:0;padding:0;");
    }
    setLanguage(label, sGerman);

    layoutTop.addWidget(&label[110]);
    layoutTop.addSpacing(65 * 11);
    layoutTop.addWidget(&label[111]);
    mainLayout.addLayout(&layoutTop);

    for(uint8_t i = 0; i < 10; i++){
        layout[i].setAlignment(Qt::AlignJustify);

        layout[i].addSpacing(65);
        for(uint8_t j = 0; j < 11; j++){
            layout[i].addWidget(&label[i * 11 + j]);
        }
        layout[i].addSpacing(65);

        mainLayout.addLayout(&layout[i]);
    }

    /*place dots*/
    layoutBottom.addWidget(&label[113]);
    layoutBottom.addSpacing(65 * 11);
    layoutBottom.addWidget(&label[112]);
    mainLayout.addLayout(&layoutBottom);

    /*place buttons*/
    button[0].setText("Left");
    button[1].setText("Middle");
    button[2].setText("Right");
    for(uint8_t i = 0; i < 3; i++){
        button[i].setStyleSheet("border:0px;color:#000000;background-color:#cccccc;margin:0;padding:10px;font-size:30px;height:40px;");
        layoutButtons.addWidget(&button[i]);
    }
    mainLayout.addLayout(&layoutButtons);

    /*place info box*/
    for(uint8_t i = 0; i < 3; i++){
        info[i].setStyleSheet("border:0px;color:#000000;background-color:#ffffff;margin:0;padding:5px;font-size:17px;font-family:Source Code Pro;");
        info[i].setText("");
        info[i].setFixedHeight(32);
        mainLayout.addWidget(&info[i]);
    }

    slider.setToolTip("simulates sunlight");
    slider.setToolTipDuration(5000);
    slider.setOrientation(Qt::Horizontal);
    slider.setMaximum(255);
    slider.setMinimum(0);
    slider.setValue(255/2);
    mainLayout.addWidget(&slider);

    this->setWindowTitle("Wordclock Simulator");
    mainLayout.setAlignment(Qt::AlignJustify);
    this->setLayout(&mainLayout);
    //this->resize(800, 1000);
    //this->setFixedSize(800, 1000);
    this->setStyleSheet("background-color:#111111;margin:0;padding:10px;");

    QTime oTime;
    oTime.restart();
    oRTC_l.u8Hours = oTime.hour();
    oRTC_l.u8Minutes = oTime.minute();
    oRTC_l.u8Seconds = oTime.second();

    Timer::setReference(&u32Timer);
    oWordclock = new Wordclock();

    createSlots();
    setInfo();

    connect(&timer, &QTimer::timeout, this, &MainWindow::loop);
    timer.start(1); //time specified in ms
}

MainWindow::~MainWindow(){
    delete oWordclock;
}

void MainWindow::setLanguage(QLabel * pLabel, QString & sLanguage){
    for(uint8_t i = 0; i < 114; i++){
        pLabel[i].setText(sLanguage.mid(i, 1));
    }
}

void MainWindow::redraw(){
    static Language oLangOld;

    setInfo();
    Language oLang = oWordclock->oSettings.getLanguage();

    QString * strLang;
    switch(oLang){
        case Language::Italian:
            strLang = &sItalian;
        break;

        case Language::German:
            strLang = &sGerman;
        break;

        case Language::Dutch:
            strLang = &sDutch;
        break;

        default:
        case Language::English:
            strLang = &sEnglish;
        break;
    }

    if(oLangOld != oLang){
        oLangOld = oLang;
        setLanguage(label, *strLang);
    }

    QPalette palette;
    QColor oColor;
    Colors * poColors;
    for(uint8_t i = 0; i < 114; i++){
        poColors = &oWordclock->oData.LEDs[i].oColors;
        oColor.setRgb(poColors->getRed(), poColors->getGreen(), poColors->getBlue());
        palette.setColor(QPalette::WindowText, oColor);
        label[i].setPalette(palette);
    }
}

void MainWindow::loop(){
    static uint32_t u32TimerOld = 0;

    u32Timer += 1000;
    if(u32Timer < u32TimerOld){
        Timer::incOverflow();
    }
    u32TimerOld = u32Timer;

    u16Seconds += 1;
    if(u16Seconds >= 1000){
        u16Seconds = 0;

        if(++oRTC_l.u8Seconds >= 60){
            oRTC_l.u8Seconds = 0;

            if(++oRTC_l.u8Minutes >= 60){
                oRTC_l.u8Minutes = 0;

                if(++oRTC_l.u8Hours >= 24){
                    oRTC_l.u8Hours = 0;
                }
            }
        }
    }

    if(oWordclock->loop()){
        redraw();
    }
}
