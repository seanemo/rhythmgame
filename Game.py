import serial
from pydub import AudioSegment, playback
import time 

class SerialMonitor:
""" Iterator that returns values read in from a serial port."""
    def __init__(self, port):
        self.port = port
        self.serialIn = serial.Serial(port)
    def __enter__(self):
        self.serialIn.flushInput()
        self.serialIn.open()
        return self
    def __exit__(self, exception_type, exception_value, exception_traceback):
        self.serialIn.close()
    def getData(self, duration):
        start_time = time.time()
        while time.time()  - start_time <= duration:
            yield self.serialIn.readline()[:-2].decode("utf-8")

class Game:
    def __init__(self, songfile):
        self.name, self.ext = songfile.split(".")
        self.audioFile = AudioSegment(f"Songs/{songfile}", format=self.ext)
        self.mode = "beatmap_gen"
    def generateBeatmap(songfile, usbModemNum):
    """Plays audio from file at songfile, and records Arduino's response through USB port to a .dat file """
        song_duration = self.audioFile.duration_seconds()
        with open(f".SongData/{name}.dat", 'w') as data_file:
            with SerialMonitor(port=usbModemNum) as usbPort: # Opening the USB port, now listening to any incoming data
                playback.play(audiofile_path) # playing audio
                for data in usbPort.getData(song_duration):
                    data_file.write(data)

    def selectiveCompilation(self):
        file_content = str()
        assert self.mode in ["beatmap_gen", "play_game"] # just checking if a valid mode is set
        if self.mode == "beatmap_gen":
            file_content = "#define GENERATE_BEATMAP 1"
        elif self.mode == "play_game":
            file_content = f""" 
            #define GENERATE_BEATMAP 0
            struct {{
                int data_len,
                int data_values[{len(self.data_vals}]
                }} SongData = {{.data_len = {len(self.data_vals}, .data_values={{{', '.join(self.data_vals)}}}
            """ 
        stream = os.popen("arduino-cli ") # this will return a stream object, so we can see the output of running this shell command (for debugging purposes)        

def playGame(songname
def main():
    cmd = cmd.lower()
    if cmd == "G":
        songname = input("Input the name of your song audio file: ")
        usb_modem_num = os.popen('''arduino-cli board list | egrep -o "usbmodem\d+"''').read()[:-2] # Grabs the first usb modem num listed, assumed arduino is on that port, minus the '\n' 
#TODO: Reorganize code a bit better
#TODO: Use string processing, arduino cli, regex, grep, etc. + grab modem num + find arduino to compile to + write to file before compilation + etc.
