#include "piorganInterface.h"
#include "piorgan.h"
#include <cmath>


PiOrganInterface* instance = NULL;

void staticOnPresetChanged(int instrument){
    instance->onPresetsChanged(instrument);
}

void staticOnSettingChanged(int setting){
    instance->onSettingChanged(setting);
}

void staticOnOffsetChanged(int offset, int instrument){
    instance->onOffsetChanged(offset, instrument);
}

PiOrganInterface::PiOrganInterface(QObject *parent) : QObject(parent)
{
    instance = this;
    myPiOrgan.setup();
    myPiOrgan.setPresetsChanged(staticOnPresetChanged); 
    myPiOrgan.setOffsetChanged(staticOnOffsetChanged);
    myPiOrgan.setSettingChanged(staticOnSettingChanged);
}



void PiOrganInterface::setRootObject(QObject *rootObject){

    eventsEnabled_ = false;

    this->rootObject = rootObject;

    pltBtnHighlight = rootObject->findChild<QObject*>("pltBtnHighlight");
    pltBtnRegular = rootObject->findChild<QObject*>("pltBtnRegular");

    txtOffsetInstrument0 = rootObject->findChild<QObject*>("txtOffsetInstrument0");
    txtOffsetInstrument1 = rootObject->findChild<QObject*>("txtOffsetInstrument1");
    txtOffsetInstrument2 = rootObject->findChild<QObject*>("txtOffsetInstrument2");

    tmbInstrument0 = rootObject->findChild<QObject*>("tmbInstrument0");
    tmbInstrument1 = rootObject->findChild<QObject*>("tmbInstrument1");
    tmbInstrument2 = rootObject->findChild<QObject*>("tmbInstrument2");

    sldVolumeInstrument0 = rootObject->findChild<QObject*>("sldVolumeInstrument0");
    sldVolumeInstrument1 = rootObject->findChild<QObject*>("sldVolumeInstrument1");
    sldVolumeInstrument2 = rootObject->findChild<QObject*>("sldVolumeInstrument2");

    sldReverbLevel = rootObject->findChild<QObject*>("sldReverbLevel");
    sldReverbRoomsize = rootObject->findChild<QObject*>("sldReverbRoomsize");
    sldReverbDamp = rootObject->findChild<QObject*>("sldReverbDamp");
    sldReverbWidth = rootObject->findChild<QObject*>("sldReverbWidth");
    sldChorusLevel = rootObject->findChild<QObject*>("sldChorusLevel");
    sldChorusDepth = rootObject->findChild<QObject*>("sldChorusDepth");
    sldChorusSpeed = rootObject->findChild<QObject*>("sldChorusSpeed");


    lvSoundFonts =  rootObject->findChild<QObject*>("lvSoundFonts");


    btnSetting[0] = rootObject->findChild<QObject*>("btnSetting0");
    btnSetting[1] = rootObject->findChild<QObject*>("btnSetting1");
    btnSetting[2] = rootObject->findChild<QObject*>("btnSetting2");
    btnSetting[3] = rootObject->findChild<QObject*>("btnSetting3");
    btnSetting[4] = rootObject->findChild<QObject*>("btnSetting4");
    btnSetting[5] = rootObject->findChild<QObject*>("btnSetting5");
    btnSetting[6] = rootObject->findChild<QObject*>("btnSetting6");
    btnSaveSetting = rootObject->findChild<QObject*>("btnSaveSetting");


    lvSoundFonts->setProperty("model",  getSoundFonts()); 
    eventsEnabled_ = true;
    int setting = myPiOrgan.getCurrentSetting();
    onSettingChanged(setting);

}

 void PiOrganInterface::setEventsEnabled(const bool enabled){
    eventsEnabled_ = enabled;
 }


void PiOrganInterface::onPresetsChanged(int instrument){
    if(eventsEnabled_){
       switch (instrument)
    {
    case 0:
        tmbInstrument0->setProperty("model",  getPresets(0)); 
        break;
    case 1:
        tmbInstrument1->setProperty("model",  getPresets(1)); 
        break;
    case 2:
        tmbInstrument2->setProperty("model",  getPresets(2)); 
        break;
    
    default:
        break;
    }
    }
}

void PiOrganInterface::onSettingChanged(int setting){
    if(eventsEnabled_){

        eventsEnabled_ = false;

        tmbInstrument0->setProperty("model",  getPresets(0)); 
        tmbInstrument1->setProperty("model",  getPresets(1)); 
        tmbInstrument2->setProperty("model",  getPresets(2)); 

        txtOffsetInstrument0->setProperty("text", QString::number(getOffset(0)));
        txtOffsetInstrument1->setProperty("text", QString::number(getOffset(1)));
        txtOffsetInstrument2->setProperty("text", QString::number(getOffset(2)));

        sldVolumeInstrument0->setProperty("value", QString::number(getVolume(0)));
        sldVolumeInstrument1->setProperty("value", QString::number(getVolume(1)));
        sldVolumeInstrument2->setProperty("value", QString::number(getVolume(2)));

        tmbInstrument0->setProperty("currentIndex",  getSelectedPreset(0)); 
        tmbInstrument1->setProperty("currentIndex",  getSelectedPreset(1)); 
        tmbInstrument2->setProperty("currentIndex",  getSelectedPreset(2)); 

        sldReverbLevel->setProperty("value", QString::number(getReverbParameter(0)));
        sldReverbRoomsize->setProperty("value", QString::number(getReverbParameter(1)));
        sldReverbDamp->setProperty("value", QString::number(getReverbParameter(2)));
        sldReverbWidth->setProperty("value", QString::number(getReverbParameter(3)));

        sldChorusLevel->setProperty("value", QString::number(getChorusParameter(0)));
        sldChorusDepth->setProperty("value", QString::number(getChorusParameter(1)));
        sldChorusSpeed->setProperty("value", QString::number(getChorusParameter(2)));

            for(int i = 0;i<7;i++){
                if(setting == i){
                btnSetting[i]->setProperty("palette",  QVariant::fromValue(pltBtnHighlight));
            }else{
                btnSetting[i]->setProperty("palette", QVariant::fromValue(pltBtnRegular));
            }
        }

        eventsEnabled_ = true;

    }
}

void PiOrganInterface::onOffsetChanged(int offset, int instrument){
    if( eventsEnabled_){
    switch (instrument)
    {
    case 0:
        txtOffsetInstrument0->setProperty("text", QString::number(offset));
        break;
    case 1:
        txtOffsetInstrument1->setProperty("text", QString::number(offset));
        break;
    case 2:
        txtOffsetInstrument2->setProperty("text", QString::number(offset));
        break;
    
    default:
        break;
    }
    }
}



void PiOrganInterface::buttonClicked(const int instrument, const QString& in)
{
    if(eventsEnabled_){
        if(in=="-"){    
            myPiOrgan.decreaseOffset(instrument);
                // offset[instrument]-=12;
                // if(offsetChanged_!=NULL){
                //     offsetChanged_(instrument,offset[instrument]);
                // }
        }else if(in=="+"){
            myPiOrgan.increaseOffset(instrument);
                    // offset[instrument]+=12;
                    // if(offsetChanged_!=NULL){
                    //     offsetChanged_(instrument,offset[instrument]);
                    // }
        }else if(in=="B"){  
                myPiOrgan.changeBank(instrument);
        }
        else if(in=="P1"){
            doSettings(0);
        }else if(in=="P2"){
            doSettings(1);
        }else if(in=="P3"){
            doSettings(2);
        }else if(in=="P4"){
            doSettings(3);
        }else if(in=="P5"){
            doSettings(4);
        }else if(in=="P6"){
            doSettings(5);
        }else if(in=="P7"){
            doSettings(6);
        }else if(in=="S"){
            printf("Save Settings(1): %d\n", saveSettings);           
            saveSettings=!saveSettings;
            setSaveMode(saveSettings);
            printf("Save Settings(2): %d\n", saveSettings);           
        }
    }
}

void PiOrganInterface::doSettings(const int setting){
    if(saveSettings){
        myPiOrgan.storeSetting(setting);
        myPiOrgan.saveSettings();
        saveSettings=false;
        setSaveMode(saveSettings); 

        for(int i = 0;i<7;i++){
            if(setting == i){
                btnSetting[i]->setProperty("palette",  QVariant::fromValue(pltBtnHighlight));
            }else{
                btnSetting[i]->setProperty("palette", QVariant::fromValue(pltBtnRegular));
            }
        }
        //onSettingChanged(setting);
    }else{
        myPiOrgan.restoreSetting(setting);
    }
}

void PiOrganInterface::sliderValueChanged(const double value, const int index)
{
    if(eventsEnabled_){
    qDebug() << "sliderValueChanged" << index << ":" << value;
        if(index<3){
            int v = std::round(value * 127);
            myPiOrgan.setVolume(v, index);
        }else if(index<7){
            myPiOrgan.setReverbParameter(value, index-3);
        }else if(index<11){
            myPiOrgan.setChorusParameter(value, index-7);
        }
    }
}


void PiOrganInterface::currentIndexChanged(const int instrument,  int index){
        if(eventsEnabled_){

    myPiOrgan.setSelectedPreset(index, instrument);
        }
}

void PiOrganInterface::soundFontChanged(const QString &itemText){
        if(eventsEnabled_){
            selectedSoundFont = itemText.toStdString();
            qDebug() << "Selected SoundFont:" << itemText;
        }
}

void PiOrganInterface::loadSoundFont(){
    if(selectedSoundFont!=""){
        qDebug() << "Load SoundFont:" << selectedSoundFont.c_str();
        myPiOrgan.loadSoundFont(selectedSoundFont.c_str());
    }
}

int PiOrganInterface::getOffset(const int instrument){
    return(myPiOrgan.getOffset(instrument));
}
double PiOrganInterface::getVolume(const int instrument){
    double volume = (myPiOrgan.getVolume(instrument) / 127.0);
    return(volume);
}

int PiOrganInterface::getSelectedPreset(const int instrument){
    return(myPiOrgan.getSelectedPreset(instrument));
}

QStringList  PiOrganInterface::getSoundFonts(){
  QStringList model;
 
    std::vector<std::string> soundFonts = myPiOrgan.getSoundFonts();

    for (const auto& sf2File : soundFonts) {
        //std::cout << sf2File << std::endl;
        model.append(QString(sf2File.c_str()));    
    }
    return model;
}


double PiOrganInterface::getReverbParameter(const int index){
  return (myPiOrgan.getReverbParameter(index));  
}

double PiOrganInterface::getChorusParameter(const int index){
  return (myPiOrgan.getChorusParameter(index));  
}

QStringList  PiOrganInterface::getPresets(const int instrument){
    QStringList model;

    int  count = myPiOrgan.getPresetsCnt(instrument);
    MPreset* presets = myPiOrgan.getPresets(instrument);
    for (int i = 0; i < count; i++)
    {
        model.append(QString(presets->name));
        //printf("%d: %s\n", i, presets->name);
        presets++;
    }
 
    return(model);
}

void PiOrganInterface::setSaveMode(bool save){

   if(save){
    btnSaveSetting->setProperty("palette",  QVariant::fromValue(pltBtnHighlight));
   }else{
     btnSaveSetting->setProperty("palette", QVariant::fromValue(pltBtnRegular));
   }
}
