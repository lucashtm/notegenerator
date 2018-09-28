
#include "notegenerator.h"
#include "ui_notegenerator.h"
#include "AudioFile.h"
#include <math.h>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioOutput>
#include <QDebug>

#include <QLoggingCategory>

QString notes[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

int keyFromFreq(double freq);

QString noteFromFreq(double freq);

double sinf(double val, double freq);

QLoggingCategory category("audio.recording");
NoteGenerator::NoteGenerator(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    this->maxFreq = 4186.009;
    this->minFreq = 27.5;
    int slider_val = ui.frequencySlider->value();
    int octave = (slider_val+8)/12;
    int note = slider_val + 8 - octave*12;
    ui.frequencyLabel->setText(QString::number(freqFromKey(slider_val))+"Hz");
    ui.noteLabel->setText(notes[note]+QString::number(octave));
    connect(ui.playButton, &QPushButton::clicked,
        [this](){ playPressed(); });
    connect(ui.frequencySlider, &QSlider::valueChanged, [this](){ sliderChanged(); });
}

void NoteGenerator::playPressed() {
    AudioFile<double> audioFile;
    AudioFile<double>::AudioBuffer buffer;
    int slider_val = ui.frequencySlider->value();
    int octave = (slider_val+8)/12;
    int note = slider_val + 8 - octave*12;
    double freq = freqFromKey(slider_val);
    buffer.resize(1);
    int buff_size = 100000;
    buffer[0].resize (buff_size);
    double inc = 1.0/44100;
    for(int i = 0; i < buff_size; i++){
        buffer[0][i] = sinf(inc*i, freq);
    }
    bool ok = audioFile.setAudioBuffer (buffer);
    audioFile.setAudioBufferSize (1, buff_size);
    audioFile.setBitDepth (24);
    audioFile.setSampleRate (44100);
    QString note_file = notes[note]+QString::number(octave)+QString(".wav");
    QByteArray ba = note_file.toUtf8();
    const char* note_str = ba.data();
    audioFile.save(note_str);
}

void NoteGenerator::sliderChanged(){
    int slider_val = ui.frequencySlider->value();
    int octave = (slider_val+8)/12;
    int note = (slider_val + 8 - octave*12)%12;
    ui.noteLabel->setText(notes[note]+QString::number(octave));
    ui.frequencyLabel->setText(QString::number(freqFromKey(slider_val))+"Hz");
}

int NoteGenerator::keyFromFreq(double freq){
    double a = 440.00;
    if(freq < 27.5 || freq > 4186.009)
        return -1;
    return round(12*log2(freq/a)+49);
}

double NoteGenerator::freqFromKey(int key){
    double a = 440.00;
    if(key < 1 || key > 88)
        return -1;
    return pow(2.0, (key-49.0)/12.0)*a;
}

QString NoteGenerator::noteFromFreq(double freq){
    int key = keyFromFreq(freq);
    if(key == -1) return "Given frequency is outside frequency range [27.5Hz, 4186.009Hz]";
    return notes[(keyFromFreq(freq)-1)%12];
}

double sinf(double val, double freq){
    return sin(2*M_PI*freq*val);
}
