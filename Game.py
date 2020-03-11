import serial
from pydub import AudioSegment, playback
import time 
import os

class SerialMonitor:
    """ Iterator that returns values read in from a serial port."""
    def __init__(self, port, baudrate):
        self.port = port
        self.serialIn = serial.Serial(port, baudrate)
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
            print("giving value")
            yield self.serialIn.readline()[:-2].decode("utf-8")

class Game:
    def __init__(self, songfile, arduino_info, path='Songs/'):
        """Making a new Game instance for a certain song and board """
        self.songname, self.ext = songfile.split(".")
        self.songPath = path+f"{songfile}"
        self.audioFile = AudioSegment.from_file(self.songPath, self.ext)
        self.port, self.fqbn, self.baudrate = arduino_info # arduino_info stores info (in a tuple) about the USB port the arduino is connected to, as well as the board model 
        # The 'fqbn' tells us information about the manufacturer, architecture, and board

    def generateBeatmap(self):
        """Plays audio from file at songfile, and records Arduino's response through USB port to a .dat file """
        self.mode = "beatmap_gen"
        song_duration = self.audioFile.duration_seconds
        with open(f"SongData/{self.songname}.dat", 'w') as data_file:
            self.selectiveCompilation()
            with serial.Serial(self.port, self.baudrate) as ser_data:
                ser_data.flushInput
                #playback.play(audiofile_path) # playing audio
                os.system(f"afplay {self.songPath}&")
                print(f"duration is {song_duration}")
                start_time = time.time()
                while time.time() - start_time <= song_duration:
                    print(f"writing data {time.time() - start_time}")
                    data_file.write(ser_data.readline()[:-2].decode("utf-8") + '\n')
                print('exited loop')

        """
            with SerialMonitor(port=self.port) as usbPort: # Opening the USB port, now listening to any incoming data
                self.selectiveCompilation()
                #playback.play(audiofile_path) # playing audio
                os.system(f"afplay {self.songPath} &")
                print(f"duration is {song_duration}")
                for data in usbPort.getData(song_duration):
                    print("writing data")
                    data_file.write(data)
"""
    def playGame(self):
        print("playing game")
        with open(f"SongData/{self.songname}.dat") as data_file:
            self.selectiveCompilation([line for line in data_file]) # feeds a list, which can then be used by the ''.join() in selectiveCompilation()
            os.system(f"afplay {self.songPath}&")

    def selectiveCompilation(self, vals=None):
        file_content = str()
        if vals is None:
            file_content = "#define GENERATE_BEATMAP 1"
        else:
            file_content = f""" 
            #define GENERATE_BEATMAP 0
            struct {{
                int data_len;
                int data_values[{3*len(vals)}];
                }} SongData = {{.data_len = {3*len(vals)}, .data_values={{{''.join(vals)[:-2]}}} }};
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

    if songfile not in curr_songs:
        raise Exception(f"Song file {songfile} not found.")
    arduino_info = (arduino_info[0], arduino_info[-2], 115200) # first part of tuple is usb modem num, and second part is the board type, last is baudrate
    print('\t'.join(str(s) for s in arduino_info))
    newgame = Game(songfile, arduino_info)
    if f"{name}.dat" not in games:
        print(f"{songfile} not found in pre-existing game files, generating new one . . . ")
        newgame.generateBeatmap()
        print("Beatmap generated!")
    
    newgame.playGame()

if __name__ == '__main__':
   main()
