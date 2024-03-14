#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <poll.h>
#include <string>
#include <algorithm>
#include <alsa/asoundlib.h>
#include <iconv.h>
#include <cstring>

#include "piorgan.h"
#include <iostream>

using namespace std;


int servo_pin_;

PiOrgan::PiOrgan(){
    cout << "PiOrgan: Do some setup..\n";
	sprintf(currentSoundFont,"%s", regular_sf);  


}

PiOrgan::PiOrgan(const char *soundFont){
	sprintf(currentSoundFont,"%s", soundFont);

    cout << "PiOrgan: Do some setup..\n";  
}

void PiOrgan::cleanup(){
    cout << "PiOrgan: Do some cleanup..\n";
	stop_ = true;
    threadMidi_->join(); 
    threadArduino_->join(); 
}

void PiOrgan::readMidi(){
int err;
    snd_seq_t *seq;
    snd_seq_open(&seq, "default", SND_SEQ_OPEN_INPUT, 0);
    snd_seq_set_client_name(seq, "MIDI_Reader");
    int port = snd_seq_create_simple_port(seq, "listen:in", SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_APPLICATION);

    // Finde das MIDI-Gerät anhand seines Namens
    const char *deviceName = "USB Uno MIDI Interface"; // Name des MIDI-Geräts
    int client = -1;
    snd_seq_client_info_t *clientInfo;
    snd_seq_client_info_alloca(&clientInfo);
    snd_seq_client_info_set_client(clientInfo, -1);

    while (snd_seq_query_next_client(seq, clientInfo) >= 0) {
        client = snd_seq_client_info_get_client(clientInfo);
        const char *name = snd_seq_client_info_get_name(clientInfo);
        if (std::strcmp(name, deviceName) == 0) {
            //std::cout << "Client: " << client << std::endl;
            break; // Das gewünschte Gerät wurde gefunden
        }
        client = -1;
    }

    if (client == -1) {
        std::cerr << "MIDI-Gerät nicht gefunden: " << deviceName << std::endl;
        return;
    }

    int status = snd_seq_connect_from(seq, port, client, 0);
    if (status < 0) {
        std::cerr << "Fehler beim Öffnen des MIDI-Geräts: " << snd_strerror(status) << std::endl;
        return;
    }

    snd_seq_event_t *ev;

	std::cerr << "MIDI-Gerät gefunden: " << deviceName << std::endl;

    while (!stop_) {
        snd_seq_event_input(seq, &ev);

		int channel = (int)ev->data.note.channel;
		int note = (int)ev->data.note.note;
		int velocity = (int)ev->data.note.velocity;

		// Bass-Split auf Channel 4
		if(channel==3){
			if(note<48){
				channel = 0;
			}else if(note>=72){
				channel = 2;
			}
			else{
				channel = 1;
			}
		}

		// Offsets anwenden
		if(channel<3){
		 note += offsets[channel];
		  velocity  =  (velocity * volumes[channel])/128;
		}

        if(ev->type!='*'){
            if (ev->type == SND_SEQ_EVENT_NOTEON) {
                //std::cout << "Note On: Channel " << channel << " Note " << note << " Velocity " << velocity;
				fluid_synth_noteon(synth,channel, note, velocity);
					
            } else if (ev->type == SND_SEQ_EVENT_NOTEOFF) {
                //std::cout << "Note Off: Channel " << channel << " Note " << note  << std::endl;
				fluid_synth_noteoff(synth, channel, note);
           }
        }
    }

			


    snd_seq_close(seq);
    return;
}


void PiOrgan::readArduino(){
	struct termios original, modified;
	char buf[255];  
	int variable;
	struct pollfd fds[1];
	int ret, res;
	int fd = -1;
	int ch;
    
	
	fd = openUSB();
	if(fd==-1){
		printf("Kein Ardunio angeschlossen.\n");
		return;
		// printf("Bitte Ardunio anschließen\n");
		// while((fd=openUSB())==-1){
		// 	printf(".");
		// 	fflush(stdout);
		// 	sleep(1);
		// }
	}else{
		printf("Verbindung zum Arduino hergestellt.\n");
	}
	
    // Speichere die ursprünglichen Terminal-Einstellungen
    tcgetattr(STDIN_FILENO, &original);
    modified = original;
    modified.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &modified);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
	
	// Start polling the arduino
	fds[0].fd = fd;
	fds[0].events = POLLRDNORM;

	while(!stop_)		// forever
	{
		ret = poll(fds, 1, 0);		//wait for response
		if (ret > 0)
		{
			/* An event on one of the fds has occurred. */
			if (fds[0].revents & POLLHUP)
			{
				printf("Hangup\n");
			}
			if (fds[0].revents & POLLRDNORM)
			{
					res = read(fd,buf,255);
					for(int i = 0; i< res; i++){
						doCmd(buf[i]);
					}
			}
		}

		
	}		
}

void  PiOrgan::setPresetsChanged(void (*fnk)(int)){
    presetsChanged_ = fnk;
}

void  PiOrgan::setOffsetChanged(void (*fnk)(int, int)){
    offsetChanged_ = fnk;
}

void  PiOrgan::setSettingChanged(void (*fnk)(int)){
    settingChanged_ = fnk;
}

void  PiOrgan::setSetSaveMode(void (*fnk)(bool)){
    setSaveMode_ = fnk;
}



void PiOrgan::storeSetting(const int setting){
	settings[setting].valid = 1;	
	settings[setting].gain = gain;	
	settings[setting].currentInstrument = currentInstrument;
	for(int i = 0;i<3;i++){
		settings[setting].selectedPresets[i] = selectedPresets[i];
		settings[setting].volumes[i] = volumes[i];
		settings[setting].offsets[i] = offsets[i];
		settings[setting].currentBank[i] = currentBank[i];	
	}

	// Read FX-Settings
	fluid_synth_get_reverb_group_level(synth, -1, &settings[setting].fxReverbParameter[0]);
	fluid_synth_get_reverb_group_roomsize(synth, -1, &settings[setting].fxReverbParameter[1]);
	fluid_synth_get_reverb_group_damp(synth, -1, &settings[setting].fxReverbParameter[2]);
	fluid_synth_get_reverb_group_width(synth, -1, &settings[setting].fxReverbParameter[3]);

	fluid_synth_get_chorus_group_level(synth, -1, &settings[setting].fxChorusParameter[0]);
	fluid_synth_get_chorus_group_depth(synth, -1, &settings[setting].fxChorusParameter[1]);
	fluid_synth_get_chorus_group_speed(synth, -1, &settings[setting].fxChorusParameter[2]);

	int chorusNr = 0;
	fluid_synth_get_chorus_group_nr(synth, -1, &chorusNr);
	settings[setting].fxControll = REVERB_ON +  CHORUS_ON + CHORUS_MOD_SINE + (chorusNr<<8);

	printf("reverb room: %f\n", settings[setting].fxReverbParameter[1]);
	printf("reverb damp: %f\n", settings[setting].fxReverbParameter[2]);
	

}

void PiOrgan::restoreSetting(const int setting){
	currentSetting = setting;
	if(settings[setting].valid){
		//gain = settings[setting].gain;	
		//fluid_synth_set_gain(synth, gain);
		currentInstrument = settings[setting].currentInstrument;
		for(int i = 0;i<3;i++){
			currentBank[i] = settings[setting].currentBank[i];
			selectedPresets[i] = settings[setting].selectedPresets[i];
			fluid_synth_bank_select(synth,channels[i],banks[currentBank[i]]);
			fluid_synth_program_change(synth, channels[i], presets[currentBank[i]][selectedPresets[i]].prog);
			printf("[%d] Selected Bank: %d - ",i + 1, banks[currentBank[i]]);
			printf("Preset %d: %s\n", selectedPresets[i], presets[currentBank[i]][selectedPresets[i]].name );

			volumes[i] = settings[setting].volumes[i];
			offsets[i] = settings[setting].offsets[i];
		}
		
		fluid_synth_reverb_on(synth, -1, (settings[setting].fxControll&REVERB_ON)?1:0);
		fluid_synth_chorus_on(synth, -1, (settings[setting].fxControll&CHORUS_ON)?1:0);
		fluid_synth_set_chorus_group_nr(synth, -1, (settings[setting].fxControll>>8) & 0xFF);
		fluid_synth_set_chorus_group_type(synth, -1, (settings[setting].fxControll&CHORUS_MOD_SINE)?FLUID_CHORUS_MOD_SINE : FLUID_CHORUS_MOD_TRIANGLE);

		fluid_synth_set_reverb_group_level(synth, -1, settings[setting].fxReverbParameter[0]);
		fluid_synth_set_reverb_group_roomsize(synth, -1, settings[setting].fxReverbParameter[1]);
		fluid_synth_set_reverb_group_damp(synth, -1, settings[setting].fxReverbParameter[2]);
		fluid_synth_set_reverb_group_width(synth, -1, settings[setting].fxReverbParameter[3]);

		fluid_synth_set_chorus_group_level(synth, -1, settings[setting].fxChorusParameter[0]);
		fluid_synth_set_chorus_group_depth(synth, -1, settings[setting].fxChorusParameter[1]);
		fluid_synth_set_chorus_group_speed(synth, -1, settings[setting].fxChorusParameter[2]);
		

		if(settingChanged_!=NULL){
			settingChanged_(setting);
			printf("Fired setting changed event: %d\n", setting);	
		}

	}else{
		printf("%d: Not a valid setting.\n", setting);
	}	
}

int PiOrgan::saveSettings(){
	// Übertrage einstellungen in die Settings
	//storeSettings();
	std::string path = currentSoundFont;
	size_t lastSlashPos = path.find_last_of('/');
    std::string fileName = path.substr(lastSlashPos + 1);
    size_t lastDotPos = fileName.find_last_of('.');
    // If a dot is found, extract substring before the dot, else use the whole filename
    if (lastDotPos != std::string::npos) {
        fileName = fileName.substr(0, lastDotPos);
    }
	char file[100];
	sprintf(file, "%s.conf",fileName.c_str());

	// Speichere die Settings 
	FILE *fp = fopen(file, "wb"); // "wb" für write binary
    if (fp == NULL) {
        fprintf(stderr, "Error writing settings:%s\n",file);
        return 0;
    }
	fwrite(&currentSetting, sizeof(int), 1, fp);
	fwrite(&settings, sizeof(struct MSetting), 10, fp);
    fclose(fp);
	
	printf("Settings written.\n");
	return 1;
}

int PiOrgan::loadSettings(){
	std::string path = currentSoundFont;
	size_t lastSlashPos = path.find_last_of('/');
    std::string fileName = path.substr(lastSlashPos + 1);
    size_t lastDotPos = fileName.find_last_of('.');
    // If a dot is found, extract substring before the dot, else use the whole filename
    if (lastDotPos != std::string::npos) {
        fileName = fileName.substr(0, lastDotPos);
    }
	char file[100];
	sprintf(file, "%s.conf",fileName.c_str());

	
	FILE *fp = fopen(file, "rb"); // "rb" für read binary

	
    if (fp == NULL) {
		currentSetting = -1;
		for(int i = 0;i<10;i++){
			settings[i].valid = 0;
		}

        fprintf(stderr, "Error opening settings:%s\n",file);
        return 0;
    }

	fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

	printf("Settings: %d\n", size);

	fread(&currentSetting, sizeof(int), 1, fp);
	if(size == 604){
		printf("Reading old setting format...\n");
		for(int i = 0; i <10 ; i++){
			fread(&settings[i], sizeof(struct MSetting_v1), 1, fp);

			// Default for all new parameter 			
			settings[i].fxControll = REVERB_ON +  CHORUS_ON + CHORUS_MOD_SINE + (0x300);
			fluid_synth_get_reverb_group_level(synth, -1, &settings[i].fxReverbParameter[0]);
			fluid_synth_get_reverb_group_roomsize(synth, -1, &settings[i].fxReverbParameter[1]);
			fluid_synth_get_reverb_group_damp(synth, -1, &settings[i].fxReverbParameter[2]);
			fluid_synth_get_reverb_group_width(synth, -1, &settings[i].fxReverbParameter[3]);
			fluid_synth_get_chorus_group_level(synth, -1, &settings[i].fxChorusParameter[0]);
			fluid_synth_get_chorus_group_depth(synth, -1, &settings[i].fxChorusParameter[1]);
			fluid_synth_get_chorus_group_speed(synth, -1, &settings[i].fxChorusParameter[2]);
		}
	}else{
		printf("Reading new setting format...\n");
		fread(&settings, sizeof(struct MSetting), 10, fp);
	}
	fclose(fp);

	printf("Settings read.\n");
	return 1;
	
}

int PiOrgan::openUSB() {
    DIR *dir;
    struct dirent *entry;
	const char *path = "/dev/";
	const char *prefix = "ttyUSB";
	char buf[255];
	int fd = -1;
	
    // Öffne das Verzeichnis
    dir = opendir(path);

    // Überprüfe, ob das Verzeichnis erfolgreich geöffnet wurde
    if (dir == NULL) {
        perror("Fehler beim Öffnen des Verzeichnisses");
    }else{
		// Durchsuche das Verzeichnis nach Einträgen
		while ((entry = readdir(dir)) != NULL) {
			// Überprüfe, ob der Eintrag mit dem Prefix beginnt
			if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0) {
				//printf();
				sprintf(buf,"%s/%s", path, entry->d_name);
				fd = open(buf, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
				struct termios options;
				tcgetattr(fd, &options);
				options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
				options.c_iflag = IGNPAR;
				options.c_oflag = 0;
				options.c_lflag = 0;
				tcflush(fd, TCIFLUSH);
				tcsetattr(fd, TCSANOW, &options);
				break;
			}
		}

		// Schließe das Verzeichnis
		closedir(dir);
	}
	return(fd);
}

int PiOrgan::getUsbSoundcardId(){
	FILE *fp;
    char buffer[255];

	int result = -1;
	

    // Befehlszeile definieren
    //const char *command = "cat /proc/asound/cards | grep \"USB-Audio\" | awk -F\" \" '{ print $1 }'";
	const char *command = "cat /proc/asound/cards | grep \"USB-Audio - USB PnP\" | awk -F\" \" '{ print $1 }'";

    // Befehl ausführen und Ergebnis lesen
    fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Fehler beim Ausführen des Befehls.\n");
        return result;
    }

    // Ergebnis verarbeiten
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Entferne Zeilenumbruch, falls vorhanden
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Wandle die Zeichenkette in eine Zahl um
        result = atoi(buffer);
    }

    // Befehl schließen
    pclose(fp);

    return result;
	
}

int PiOrgan::geHifiberryCardId(){
FILE *fp;
    char buffer[255];

	int result = -1;
	

    // Befehlszeile definieren
    const char *command = "cat /proc/asound/cards | grep \"sndrpihifiberry\" | awk -F\" \" '{ print $1 }'";

    // Befehl ausführen und Ergebnis lesen
    fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Fehler beim Ausführen des Befehls.\n");
        return result;
    }

    // Ergebnis verarbeiten
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Entferne Zeilenumbruch, falls vorhanden
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Wandle die Zeichenkette in eine Zahl um
        result = atoi(buffer);
    }

    // Befehl schließen
    pclose(fp);

    return result;
}
void convertISO88591ToUTF8(const char* isoString,  char* utf8Buffer) {
    // Öffnen des Konvertierungspuffers
    iconv_t cd = iconv_open("UTF-8", "ISO-8859-1");

    // Größe des Eingabestrings
    size_t in_size = std::strlen(isoString);

    // Berechnung der maximal möglichen Größe des Ausgabestrings
    size_t out_size = in_size * 4; // Maximal 4 Byte pro Zeichen in UTF-8

    // Allokation des Ausgabestrings
    char* utf8Ptr = utf8Buffer;

    // Konvertierung
    size_t result = iconv(cd, const_cast<char**>(&isoString), &in_size, &utf8Ptr, &out_size);

    // Schließen des Konvertierungspuffers
    iconv_close(cd);

	int len = utf8Ptr - utf8Buffer;

	utf8Buffer[len] = 0;
	utf8Buffer[len+1] = 0;

}

void PiOrgan::loadPresets(){
    fluid_preset_t* preset = NULL;
	
	cntBanks = 0;
	int lastBanknum = -1;
	int bank;
	
	fluid_sfont_iteration_start(sfont);
	while (preset = fluid_sfont_iteration_next(sfont)) {
		int banknum = fluid_preset_get_banknum(preset);
		int prog = fluid_preset_get_num(preset);
		const char* name = fluid_preset_get_name(preset);
		int i =0;
		char buffer2[200];
		convertISO88591ToUTF8(name, buffer2);

		if(lastBanknum!=banknum){
			banks[cntBanks] = banknum;
			if(cntBanks>0)printf("%d. Bank: %d presets loaded.\n", cntBanks, cntPresets[bank] );
			bank = cntBanks;
			cntBanks++;
			cntPresets[bank] = 0;
			lastBanknum = banknum;
		}
		presets[bank][cntPresets[bank]].bank = banknum;
		presets[bank][cntPresets[bank]].prog = prog;
		//sprintf(presets[bank][cntPresets[bank]].name,"%s",name);
		sprintf(presets[bank][cntPresets[bank]].name,"%s",buffer2);



		//printf("%d - %d: %s\n",banknum,prog,name);
		cntPresets[bank]++;
	}
	printf("%d. Bank: %d presets loaded.\n", cntBanks, cntPresets[bank] );
	
}

void PiOrgan::loadSoundFont(const char *soundfont){

	sprintf(currentSoundFont, "/usr/share/sounds/sf2/%s", soundfont);

	if(sfont_id!=-1) fluid_synth_sfunload(synth, sfont_id, 1);
	sfont_id = fluid_synth_sfload(synth,currentSoundFont, 1);
	if(sfont_id == FLUID_FAILED)
	{
		puts("Loading the SoundFont failed!");
	}
	sfont = fluid_synth_get_sfont_by_id(synth, sfont_id);
	loadPresets();
	loadSettings();
	if(settingChanged_!=NULL){
		settingChanged_(currentSetting);
	}
}


int PiOrgan::openSynth(const char *soundfont){
	fluid_settings_t *settings = NULL;
    //fluid_synth_t *synth = NULL;
    fluid_audio_driver_t *adriver = NULL;

    
    int sfont_id;
    int i;
    int idHB, idUsb;
	int res;
 
    /* Create the settings. */
    settings = new_fluid_settings();
    if(settings == NULL)
    {
        puts("Failed to create the settings!");
        goto err_openSynth;
    }
 
    /* Change the settings if necessary*/
	res = fluid_settings_setstr(settings, "audio.driver", "alsa");

	idHB = geHifiberryCardId();
	idUsb = getUsbSoundcardId();
	
	if(idUsb==-1 && idHB==-1){
        puts("No USB-Soundcard or HifiBerry found!");
        //goto err_openSynth;
	}else{
		char buffer[50];
		if(idHB!=-1){
			printf("Found HifiBerry: %d\n", idHB);
			sprintf(buffer,"hw:%d,0", idHB);
		}else if(idUsb!=-1){
			printf("Found USB-Audio: %d\n", idUsb);
			sprintf(buffer,"hw:%d,0", idUsb);
			res = fluid_settings_setnum(settings, "synth.sample-rate", 48000);
		}
		res = fluid_settings_setstr(settings, "audio.alsa.device", buffer);
	}

    /* Create the synthesizer. */
    synth = new_fluid_synth(settings);
    if(synth == NULL)
    {
        puts("Failed to create the synth!");
        goto err_openSynth;
    }
 
    /* Load a SoundFont and reset presets (so that new instruments
     * get used from the SoundFont)
     * Depending on the size of the SoundFont, this will take some time to complete...
     */
    sfont_id = fluid_synth_sfload(synth,soundfont, 1);
    if(sfont_id == FLUID_FAILED)
    {
        puts("Loading the SoundFont failed!");
        goto err_openSynth;
    }
 
    /* Create the audio driver. The synthesizer starts playing as soon
       as the driver is created. */
    adriver = new_fluid_audio_driver(settings, synth);
    if(adriver == NULL)
    {
        puts("Failed to create the audio driver!");
        goto err_openSynth;
    }
    
    /* Iterate soundfont's presets */
	sfont = fluid_synth_get_sfont_by_id(synth, sfont_id);
	if (sfont) {
       const char* name = fluid_sfont_get_name(sfont);
        printf("Soundfont: %s\n", name);
		loadPresets();
	}
    
    //fluid_synth_program_change(synth, 0, 19);
	return 1;

err_openSynth:
    /* Clean up */
    delete_fluid_audio_driver(adriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
 
    return 0;
	
}



void PiOrgan::doCmd(char cmd){
	int noteOn = (cmd<128);
	int key = (cmd<128)?cmd:cmd-128;


	if(keymapping[key].instrument != -1){

		int instrument = keymapping[key].instrument;
		int note = keymapping[key].note + offsets[instrument];
		int channel = channels[instrument];
		int volume = volumes[instrument];

		if(noteOn){
			printf("%d -> [%d]: %d on\n", key, instrument + 1, note);
			fluid_synth_noteon(synth, channel, note, volume);
		}else{
			printf("%d -> [%d]: %d off\n", key, instrument + 1, note);
			fluid_synth_noteoff(synth, channel, note);
		}		
	}else{
		if(noteOn){
			printf("unmapped key: %d on\n", key);
		}else {
			printf("unmapped key: %d off\n", key);
		}
	}

	/*
	int instrument = 0;
	int note = 59 + key + offsets[instrument];
	int channel = channels[instrument];
	int volume = volumes[instrument];


	if(noteOn){
		fluid_synth_noteon(synth, channel, note, volume);
	}else{
		fluid_synth_noteoff(synth, channel, note);
	}
	*/
	
} 

void PiOrgan::setVolume(int volume, int instrument){
	volumes[instrument]=volume;
	printf("[%d] Volume: %d\n", instrument + 1,  volumes[instrument]);
}

int PiOrgan::getVolume(int instrument){
	return(volumes[instrument]);
}

void PiOrgan::setReverbParameter(double value, int index){
	 
	fluid_synth_reverb_on(synth, -1, 1);

	switch (index)
	{
	case 0:
		printf("Set reverb level: %f\n", value); // 0..1
		fluid_synth_set_reverb_group_level(synth,-1, value);
		break;	
	case 1:
		printf("Set reverb room-size: %f\n", value*1.2); // 0-1.2
		fluid_synth_set_reverb_group_roomsize(synth,-1, value*1.2);
		break;
	case 2:
		printf("Set reverb damp: %f\n", value); // 0-1
		fluid_synth_set_reverb_group_damp(synth,-1, value);
		break;	
	case 3:
		printf("Set reverb width: %f\n", value*100); // 0-100
		fluid_synth_set_reverb_group_width(synth,-1, value*100);
		break;
	}
}

double PiOrgan::getReverbParameter(int index){
	double result = 0;
	switch (index)
	{
	case 0:
		fluid_synth_get_reverb_group_level(synth,-1, &result);
		printf("Reverb level: %f\r\n",result);
		break;
	case 1:
		fluid_synth_get_reverb_group_roomsize(synth,-1, &result);
		printf("Reverb room size: %f\r\n",result);
		result = result / 1.2;
		break;
	case 2:
		fluid_synth_get_reverb_group_damp(synth,-1, &result);
		printf("Reverb damp: %f\r\n",result);
		break;
	case 3:
		fluid_synth_get_reverb_group_width(synth,-1, &result);
		printf("Reverb width: %f\r\n",result);
		result = result / 100;
		break;
	}
	return result;
}

void PiOrgan::setChorusParameter(double value, int index){
	 
	fluid_synth_chorus_on(synth, -1, 1);

	switch (index)
	{
	case 0:
		printf("Set chorus level: %f\n", value*10); // 0-10
		fluid_synth_set_chorus_group_level(synth,-1, value * 10);
		break;	
	case 1:
		printf("Set chorus depth: %f\n", value*21); // 0-21
		fluid_synth_set_chorus_group_depth(synth,-1, value * 21);
		break;
	case 2:
		printf("Set chorus speed: %f\n", value*5); //0.1 - 5
		fluid_synth_set_chorus_group_speed(synth,-1, value * 5);
		break;	
	}
}

double PiOrgan::getChorusParameter(int index){
	double result = 0;
	switch (index)
	{
	case 0:
		fluid_synth_get_chorus_group_level(synth,-1, &result);
		printf("Chorus level: %f\r\n",result);
		result = result /10;
		break;
	case 1:
		fluid_synth_get_chorus_group_depth(synth,-1, &result);
		printf("Chorus depth: %f\r\n",result);
		result = result / 21;
		break;
	case 2:
		fluid_synth_get_chorus_group_speed(synth,-1, &result);
		printf("Chorus speed: %f\r\n",result);
		result = result / 5;
		break;
	}
	return result;
}

void PiOrgan::setOffset(int offset, int instrument){
	offsets[instrument]=offset;
	if(offsetChanged_!=NULL){
		offsetChanged_(offsets[instrument], instrument);
	}
	fluid_synth_all_notes_off(synth, channels[instrument]);
	printf("[%d] Offset: %d\n", instrument + 1,  offsets[instrument]);
}

int PiOrgan::getOffset(int instrument){
	return(offsets[instrument]);
}

void PiOrgan::increaseOffset( int instrument){
	 if(offsets[instrument]<MAX_OFFSET){
			offsets[instrument]+=12;
			if(offsetChanged_!=NULL){
				offsetChanged_(offsets[instrument], instrument);
			}
			fluid_synth_all_notes_off(synth, channels[instrument]);
			printf("[%d] Offset: %d\n", instrument + 1,  offsets[instrument]);
		 }
}

void PiOrgan::decreaseOffset( int instrument){
 	if(offsets[instrument]>MIN_OFFSET){
		offsets[instrument]-=12;
		if(offsetChanged_!=NULL){
			offsetChanged_(offsets[instrument], instrument);
		}
		fluid_synth_all_notes_off(synth, channels[instrument]);
		printf("[%d] Offset: %d\n", instrument + 1,  offsets[instrument]);
	}
}

void PiOrgan::setInstrument(int instrument){
	if(currentInstrument != instrument){
		currentInstrument = instrument;
		printf("[%d] Changed Instrument!\n", currentInstrument + 1);
	}
}

void PiOrgan::setSelectedPreset(int preset, int instrument){
	selectedPresets[instrument]=preset;			
	fluid_synth_program_change(synth, channels[instrument], presets[currentBank[instrument]][selectedPresets[instrument]].prog);
	printf("[%d] Preset %d: %s\n", instrument + 1,  selectedPresets[instrument], presets[currentBank[instrument]][selectedPresets[instrument]].name );
}

int PiOrgan::getSelectedPreset(int instrument){
	return(selectedPresets[instrument]);			
}

int PiOrgan::getCurrentSetting(){
	return(currentSetting);
}

void PiOrgan::keyPressed(int key){
	//printf("Key: %d\n", key);
switch(key){
	case LEFT:
		if(selectedPresets[currentInstrument]>0){
			setSelectedPreset(selectedPresets[currentInstrument]-1, currentInstrument);
		}
	break;
	
	case RIGHT:
		if(selectedPresets[currentInstrument]<cntPresets[currentBank[currentInstrument]]-1){
			setSelectedPreset(selectedPresets[currentInstrument]+1, currentInstrument);
		}
	
	break;
	
	case UP:
		increaseOffset(currentInstrument);
		break;
	case DOWN:
		decreaseOffset(currentInstrument);
		break;
	case PGUP:
		 if(volumes[currentInstrument]<127){
			volumes[currentInstrument]+=8;
			if(volumes[currentInstrument]>127)volumes[currentInstrument]=127;
			printf("[%d] Volume: %d\n", currentInstrument + 1,  volumes[currentInstrument]);
		 }
		break;
	case PGDOWN:
		 if(volumes[currentInstrument]>0){
			volumes[currentInstrument]-=8;
			if(volumes[currentInstrument]<0)volumes[currentInstrument]=0;
			printf("[%d] Volume: %d\n", currentInstrument + 1,  volumes[currentInstrument]);
		 }
		break;
}


}

int PiOrgan::setup() {
	loadSettings();


		if(!openSynth(currentSoundFont)){
			return EXIT_FAILURE;
		}
		restoreSetting(0);

	// Start Arduino- und Midi-Thread
	threadArduino_ = new thread(&PiOrgan::readArduino, this);
	threadMidi_ = new thread(&PiOrgan::readMidi, this);

	return EXIT_SUCCESS;
}

std::vector<std::string> PiOrgan::getSoundFonts() {
    std::vector<std::string> sf2Files;
	string directoryPath= "/usr/share/sounds/sf2";

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".sf2") {
            sf2Files.push_back(entry.path().filename().string());
        }
    }

	// Sortiere den Vektor
    std::sort(sf2Files.begin(), sf2Files.end());

    return sf2Files;
}


MPreset* PiOrgan::getPresets(int instrument){
	return (presets[currentBank[instrument]]);
}

int PiOrgan::getPresetsCnt(int instrument){
	return (cntPresets[currentBank[instrument]]);
}

int PiOrgan::changeBank(int instrument){
					currentBank[instrument]++;
					if(currentBank[instrument]>=cntBanks)
					currentBank[instrument] = 0;	
					printf("[%d] Changed Bank to : %d\n",instrument + 1, banks[currentBank[instrument]]);
					fluid_synth_bank_select(synth,channels[instrument],banks[currentBank[instrument]]);
					setSelectedPreset(0, instrument);
					if(presetsChanged_!=NULL){
						presetsChanged_(instrument);
					}
					return currentBank[instrument];
}

int PiOrgan::loop() {
	struct termios original, modified;
	char buf[255];  
	int variable;
	struct pollfd fds[1];
	int ret, res;
	int fd = -1;
	int ch;
    
	
	fd = openUSB();
	if(fd==-1){
		printf("Bitte Ardunio anschließen\n");
		while((fd=openUSB())==-1){
			printf(".");
			fflush(stdout);
			sleep(1);
		}
	}else{
		printf("Verbindung zum Arduino hergestellt.\n");
	}
	
	// if(argc==2){
	// 	if(!openSynth( argv[1])){
	// 		return EXIT_FAILURE;
	// 	}
	// }else{
	// }	

    // Speichere die ursprünglichen Terminal-Einstellungen
    tcgetattr(STDIN_FILENO, &original);
    modified = original;
    modified.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &modified);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
	
	// Start polling the arduino
	fds[0].fd = fd;
	fds[0].events = POLLRDNORM;

	while(1)		// forever
	{
		ret = poll(fds, 1, 0);		//wait for response
		if (ret > 0)
		{
			/* An event on one of the fds has occurred. */
			if (fds[0].revents & POLLHUP)
			{
				printf("Hangup\n");
			}
			if (fds[0].revents & POLLRDNORM)
			{
					res = read(fd,buf,255);
					for(int i = 0; i< res; i++){
						doCmd(buf[i]);
						//printf("%d\n", buf[i]);
						//fflush(stdout);
					}
			}
		}

		// Check Keyboard
	    ch = getchar();
	    if (ch != EOF) {
            if (ch == '[') {
                ch = getchar();
                switch (ch) {
                    case 'A':
                        keyPressed(UP);
                        break;
                    case 'B':
                        keyPressed(DOWN);
                        break;
                    case 'C':
                        keyPressed(RIGHT);
                        break;
                    case 'D':
                        keyPressed(LEFT);
                        break;
                    case '5':
                        keyPressed(PGUP);
                        break;
                    case '6':
                    case '2':
                        keyPressed(PGDOWN);
                        break;
                    default:
                        // Anderes Zeichen nach Escape, ignoriere es
						printf("Escape: %d\n", ch);
                        break;
                }
            } else {
				switch (ch){
					case '1':
					setInstrument(0);
					break;
					case '2':
					setInstrument(1);
					break;
					case '3':
					setInstrument(2);
					break;
					case ']':
					keyPressed(PGUP);
					break;
					case '/':
					keyPressed(PGDOWN);
					break;
					case 'g':
						gain +=0.5;
						if(gain>10) gain = 0.5;
						fluid_synth_set_gain(synth, gain);
						printf("Set gain to: %4.1f\n", gain);
					break;
					case 's':
						saveSettings();
					break;
					case 'o':
					// if(currentSoundFont!=0){
					// 	storeSettings();
					// 	currentSoundFont =0;
					// 	restoreSettings();
					// 	loadSoundFont(organ_sf);
					// 	restoreSettings();
					// 	//currentBank[currentInstrument] = 0;
					// 	//selectPreset(0);
					// }
					break;
					case 'p':
					// if(currentSoundFont!=1){
					// 	storeSettings();
					// 	currentSoundFont =1;
					// 	loadSoundFont(regular_sf);
					// 	restoreSetting(currentSetting);
					// 	//currentBank[currentInstrument] = 0;
					// 	//selectPreset(0);
					// }
					break;
					case 'b':
						changeBank(currentInstrument);
					break;
				}
                // Ausgabe der erkannten Taste auf dem Bildschirm
                //printf("Erkannte Taste: %d\n", ch);
            }
        }		
	}
	
    return EXIT_SUCCESS;
}
