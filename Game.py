import serial
from pydub import AudioSegment, playback
import time 
import os

class SerialMonitor:
    """ Iterator that returns values read in from a serial port."""
    def __init__(self, port):
        self.port = port
        self.serialIn = serial.Serial(port)
    def __enter__(self):
        self.serialIn.flushInput()
#        self.serialIn.open()
        return self
    def __exit__(self, exception_type, exception_value, exception_traceback):
        self.serialIn.close()
        return True # any exception that may have occurred within a context manager is 
    def getData(self, duration):
        start_time = time.time()
        while time.time()  - start_time <= duration:
            yield self.serialIn.readline()[:-2].decode("utf-8")

class Game:
    def __init__(self, songfile, arduino_info):
        """Making a new Game instance for a certain song and board """
        self.songname, self.ext = songfile.split(".")
        self.audioFile = AudioSegment.from_file(f"Songs/{songfile}", self.ext)
        self.port, self.fqbn = arduino_info # arduino_info stores info (in a tuple) about the USB port the arduino is connected to, as well as the board model 
        # The 'fqbn' tells us information about the manufacturer, architecture, and board

    def generateBeatmap(self):
        """Plays audio from file at songfile, and records Arduino's response through USB port to a .dat file """
        self.mode = "beatmap_gen"
        song_duration = self.audioFile.duration_seconds
        with open(f"SongData/{self.songname}.dat", 'w') as data_file:
            with SerialMonitor(port=self.port) as usbPort: # Opening the USB port, now listening to any incoming data
                self.selectiveCompilation()
                playback.play(audiofile_path) # playing audio
                for data in usbPort.getData(song_duration):
                    data_file.write(data)

    def playGame(self):
        with open(f"SongData/{self.songname}.dat") as data_file:
            self.selectiveCompilation([line for line in data_file]) # feeds a list, which can then be used by the ''.join() in selectiveCompilation()

    def selectiveCompilation(self, vals=None):
        file_content = str()
        if vals is None:
            file_content = "#define GENERATE_BEATMAP 1"
        else:
            file_content = f""" 
            #define GENERATE_BEATMAP 0
            struct {{
                int data_len,
                int data_values[{len(vals)}]
                }} SongData = {{.data_len = {len(vals)}, .data_values={{{', '.join(vals)}}} }}
            """
        with open("CurrentSongData.h", 'w') as currSongData:
            currSongData.write(file_content)
        compile_msg = os.popen(f"arduino-cli compile -b {self.fqbn}").read() # executes said terminal command, and returns string of output 
        export_msg = os.popen(f"arduino-cli upload -b {self.fqbn} -p {self.port}").read() # same as above
        print(compile_msg + '\n' + export_msg)

def main():
    curr_songs = os.listdir("Songs")
    games = os.listdir("SongData")
    songfile = input("Input the name of your song audio file: ")
    name = songfile.split(".")[0]
    arduino_info = os.popen('''arduino-cli board list | egrep "usbmodem"''').read().split() # Grabs the first usb modem num listed, assumed arduino is on that port, minus the '\n' 
    print(arduino_info)

    if songfile not in curr_songs:
        raise Exception(f"Song file {songfile} not found.")
    arduino_info = (arduino_info[0], arduino_info[-2]) # first part of tuple is usb modem num, and second part is the board type
    newgame = Game(songfile, arduino_info)
    if name not in games:
        print(f"{songfile} not found in pre-existing game files, generating new one . . . ")
        newgame.generateBeatmap()
        print("Beatmap generated!")
    
    newgame.playGame()

if __name__ == '__main__':
   main()
