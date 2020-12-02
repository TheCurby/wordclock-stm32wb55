#pragma once

#include "wordclock/wordclock.hpp"
#include <QtWidgets>
#include <QTime>
#include <QTimer>

class MainWindow : public QWidget{
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

    private:
        void setInfo();
        void createSlots();
        void setLanguage(QLabel * pLabel, QString & sLanguage);

        QVBoxLayout mainLayout;
        QHBoxLayout layout[10];
        QHBoxLayout layoutTop, layoutBottom, layoutButtons;
        QLabel label[114];
        QPushButton button[3];
        QLabel info[3];
        QSlider slider;

        QTimer timer;
        Wordclock * oWordclock;

        static QString sGerman;
        static QString sDutch;
        static QString sEnglish;
        static QString sItalian;

    public slots:
        void redraw();

    private slots:
        void loop();
        void on_ButtonLeft_pressed();
        void on_ButtonLeft_released();
        void on_ButtonMiddle_pressed();
        void on_ButtonMiddle_released();
        void on_ButtonRight_pressed();
        void on_ButtonRight_released();
        void on_slider_sliderMoved(int position);
};
