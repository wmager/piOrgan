#ifndef MYCLASS_H
#define MYCLASS_H

#include <QDebug>
#include <QObject>
#include <QtWidgets>

#include <string>
#include <piorgan.h>

class PiOrganInterface : public QObject
{
    Q_OBJECT

public:
    explicit PiOrganInterface(QObject *parent = 0);
    void onPresetsChanged(int instrument);
    void onSettingChanged(int setting);
    void onOffsetChanged(int offset, int instrument);
    void setPresetsChanged(void (*fnk)(int));
    void setSaveMode(bool save);
    QStringList getPresets(const int instrument);
    QStringList getSoundFonts();
    int getOffset(const int instrument);
    double getVolume(const int instrument);
    int getSelectedPreset(const int instrument);
    double getReverbParameter(int index);
    double getChorusParameter(int index);
    void setEventsEnabled(const bool enabled);
    void setRootObject(QObject *rootObject);

public slots:
   void buttonClicked(const int instrument, const QString& in);
   void sliderValueChanged(const double value, const int instrument);
   void currentIndexChanged(const int instrument, int index);
   void soundFontChanged(const QString &itemText);
   void loadSoundFont();

private :
    void doSettings(const int setting);
    bool eventsEnabled_ = false;
    PiOrgan myPiOrgan; 
    bool saveSettings = false;
    std::string selectedSoundFont = ""; 
    int currentSetting = -1;

    QObject *rootObject;

    QObject *txtOffsetInstrument0;
    QObject *txtOffsetInstrument1;
    QObject *txtOffsetInstrument2;

    QObject *tmbInstrument0;
    QObject *tmbInstrument1;
    QObject *tmbInstrument2;

    QObject *sldVolumeInstrument0;
    QObject *sldVolumeInstrument1;
    QObject *sldVolumeInstrument2;

    QObject *sldReverbLevel;
    QObject *sldReverbRoomsize;
    QObject *sldReverbDamp;
    QObject *sldReverbWidth;

    QObject *sldChorusLevel;
    QObject *sldChorusDepth;
    QObject *sldChorusSpeed;

    QObject *lvSoundFonts;

    QObject *btnSetting[7];
    QObject *btnSaveSetting;

    QObject *pltBtnHighlight;
    QObject *pltBtnRegular;

};

#endif // MYCLASS_H
