import serial
from pydub import AudioSegment, playback
import time
import os

class Game:
    def __init__(self, songfile, arduino_info, path='Songs/'):
        """Making a new Game instance for a certain song and board """
        self.songname, self.ext = songfile.split(".")
        self.songPath = path+f"{songfile}"
        self.audioFile = AudioSegment.from_file(self.songPath, self.ext)
        self.port, self.fqbn, self.baudrate = arduino_info # arduino_info stores info (in a tuple) about the USB port the arduino is connected to, as well as the board model
        # The 'fqbn' tells us information about the manufacturer, architecture, and board

    def generateBeatmap(self):
        """Plays audio from file at songfile, and records Arduino's response through USB port to a .csv file """
        self.mode = "beatmap_gen"
        song_duration = self.audioFile.duration_seconds
        with open(f"SongData/{self.songname}.csv", 'w') as data_file:
            self.selectiveCompilation()
            with serial.Serial(port=self.port, baudrate=self.baudrate, timeout=song_duration) as ser_data:
                ser_data.flushInput()
                os.system(f"afplay {self.songPath}&") #playing audio in background
                print(f"duration is {song_duration}")
                start_time = time.time()
                while time.time() - start_time <= song_duration:
                    data = ser_data.readline()[:-2].decode("utf-8")
                    print(f"writing {data} at {time.time() - start_time}")
                    data_file.write(data + '\n')
                print('exited loop')

    def playGame(self):
        print("\nplaying game\n")
        with open(f"SongData/{self.songname}.csv") as data_file:
            self.selectiveCompilation([line for line in data_file]) # feeds a list, which can then be used by the ''.join() in selectiveCompilation()
            os.system(f"afplay {self.songPath}&") #playing audio in background
            # Reading in values to test led code
            with serial.Serial(port=self.port, baudrate=self.baudrate, timeout=self.audioFile.duration_seconds) as ser_data:
                ser_data.flushInput()
                start_time = time.time()
                while time.time() - start_time <= self.audioFile.duration_seconds:
                    data = ser_data.readline()[:-2].decode("utf-8")
                    print(f"reading {data} at {time.time() - start_time}")
                print('exited loop')
    def selectiveCompilation(self, vals=None):
        file_content = str()
        if vals is None:
            file_content = "#define GENERATE_BEATMAP 1"
        else:
            file_content = f"""
#define GENERATE_BEATMAP 0
extern int DelayData[{len(vals)}] = {{{', '.join(vals)}}};
extern int DelayDataLen = {len(vals)};
            """
            print(f"Making array with {len(vals)} ints\n")
        with open("CurrentSongData.h", 'w') as currSongData:
            currSongData.write(file_content)
        compile_msg = os.popen(f"arduino-cli compile -b {self.fqbn}").read() # executes said terminal command, and returns string of output
        export_msg = os.popen(f"arduino-cli upload -b {self.fqbn} -p {self.port}").read() # same as above
        print(compile_msg + '\n' + export_msg)

def main():
    os.system("osascript -e \"set Volume 7\"")
    curr_songs = os.listdir("Songs")
    games = os.listdir("SongData")
    songfile = input("Input the name of your song audio file: ")
    name = songfile.split(".")[0]
    arduino_info = os.popen('''arduino-cli board list | egrep "usbmodem"''').read().split() # Grabs the first usb modem num listed, assumed arduino is on that port, minus the '\n'

    if songfile not in curr_songs:
        raise Exception(f"Song file {songfile} not found.")
    try:
        arduino_info = (arduino_info[0], arduino_info[-2], 115200) # first part of tuple is usb modem num, and second part is the board type, last is baudrate
    except IndexError:
        raise Exception("Arduino not connected.")
    print('\t'.join(str(s) for s in arduino_info))
    newgame = Game(songfile, arduino_info)
    if f"{name}.csv" not in games:
        print(f"{songfile} not found in pre-existing game files, generating new one . . . ")
        newgame.generateBeatmap()
        print("Beatmap generated!")
    newgame.playGame()

if __name__ == '__main__':
   main()
