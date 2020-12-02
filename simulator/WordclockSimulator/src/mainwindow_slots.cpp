#include "mainwindow.h"

bool bLeftButton = true;
bool bMiddleButton = true;
bool bRightButton = true;
uint8_t u8Light = 50;

void MainWindow::createSlots(){
    connect(&button[0], &QPushButton::pressed, this, &MainWindow::on_ButtonLeft_pressed);
    connect(&button[0], &QPushButton::released, this, &MainWindow::on_ButtonLeft_released);
    connect(&button[1], &QPushButton::pressed, this, &MainWindow::on_ButtonMiddle_pressed);
    connect(&button[1], &QPushButton::released, this, &MainWindow::on_ButtonMiddle_released);
    connect(&button[2], &QPushButton::pressed, this, &MainWindow::on_ButtonRight_pressed);
    connect(&button[2], &QPushButton::released, this, &MainWindow::on_ButtonRight_released);
    connect(&slider, &QSlider::valueChanged, this, &MainWindow::on_slider_sliderMoved);
    //connect(&slider, &QSlider::mo, this, &MainWindow::on_slider_move)
}

void MainWindow::setInfo(){
    QString sLeft = "";
    QString sMiddle = "";
    QString sRight = "";

    switch(oWordclock->CurrentMenu){
        default:
        break;

        case Menu::Clock:
            sLeft = "go to menu -> DCF77";
            sMiddle = "go to seconds screen";
            sRight = "disable / enable output";
        break;

        case Menu::Seconds:
            sLeft = "go to menu -> DCF77";
            sMiddle = "go to temperature screen";
            sRight = "disable / enable output";
        break;

        case Menu::Temp:
            sLeft = "go to menu -> DCF77";
            sMiddle = "go to clock screen";
            sRight = "disable / enable output";
        break;

        case Menu::DCF77:
            sLeft = "go to menu -> set hour";
            sMiddle = "enable / disable DCF77 receiver";
            sRight = "enable / disable DCF77 receiver";
        break;

        case Menu::Hours:
            sLeft = "go to menu -> set minutes";
            sMiddle = "press / hold to increase hour";
            sRight = "press / hold to decrease hour";
        break;

        case Menu::Minutes:
            sLeft = "go to menu -> set color";
            sMiddle = "press / hold to increase minutes";
            sRight = "press / hold to decrease minutes";
        break;

        case Menu::Colors:
            sLeft = "go to menu -> brightness adjustment";

            switch(oWordclock->u8SubMenu){
                case 1:
                    sMiddle = "select green";
                    sRight = "press double to reset; hold down to increase value";
                break;

                case 2:
                    sMiddle = "select blue";
                    sRight = "press double to reset; hold down to increase value";
                break;

                case 3:
                    sMiddle = "select color mode";
                    sRight = "press double to reset; hold down to increase value";
                break;

                default:
                case 0:
                    sMiddle = "select red";
                    sRight = "press to change color mode";
                break;
            }
        break;

        case Menu::Light:
            sLeft = "go to menu -> set animation";

            switch(oWordclock->u8SubMenu){
                default:
                case 0:
                    sMiddle = "select minimum";
                    sRight = "press double to reset max; hold down to decrease value";
                break;

                case 1:
                    sMiddle = "select maximum";
                    sRight = "press double to reset min; hold down to decrease value";
                break;
            }
        break;

        case Menu::Animation:
            sLeft = "go to menu -> set night mode";
            sMiddle = "press to change animation";
            sRight = "press to change animation";
        break;

        case Menu::Night:
            sLeft = "go to menu -> set language";

            switch(oWordclock->u8SubMenu){
                default:
                case 0:
                    sMiddle = "select stop time";
                    sRight = "press / hold to set start time; press double to change direction";
                break;

                case 1:
                    sMiddle = "select enable screen";
                    sRight = "press / hold to set stop time; press double to change direction";
                break;

                case 2:
                    sMiddle = "select start time";
                    sRight = "enable / disable night mode";
                break;
            }
        break;

        case Menu::Lang:
            sLeft = "go to clock main screen";
            sMiddle = "press to change language";
            sRight = "press to change language";
        break;
    }

    info[0].setText("Left: " + sLeft);
    info[1].setText("Middle: " + sMiddle);
    info[2].setText("Right: " + sRight);
}

void MainWindow::on_ButtonLeft_pressed(){
    bLeftButton = false;
}

void MainWindow::on_ButtonLeft_released(){
    bLeftButton = true;
}

void MainWindow::on_ButtonMiddle_pressed(){
    bMiddleButton = false;
}

void MainWindow::on_ButtonMiddle_released(){
    bMiddleButton = true;
}

void MainWindow::on_ButtonRight_pressed(){
    bRightButton = false;
}

void MainWindow::on_ButtonRight_released(){
    bRightButton = true;
}

void MainWindow::on_slider_sliderMoved(int position){
    u8Light = position;
}
