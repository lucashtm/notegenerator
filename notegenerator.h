#pragma once

#include <QMainWindow>
#include "ui_notegenerator.h"

class NoteGenerator : public QMainWindow
{
    Q_OBJECT
    static int keyFromFreq(double freq);
    static QString noteFromFreq(double freq);
    static double freqFromKey(int key);
public:
    double minFreq;
    double maxFreq;
    double frequency;
    explicit NoteGenerator(QWidget *parent = nullptr);
    ~NoteGenerator() override = default;
    void playPressed();
    void sliderChanged();

private:
    Ui::NoteGenerator ui;
};
