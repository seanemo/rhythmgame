## Rhythm Game Project:


#### Proposed Idea for Flow of code and all that

1. Audio Source sends audio signal (AS) to Audio Chip, and sends delayed audio signal to Speaker
2. Audio Chip processes AS, sends strengths of frequency bands to Arduino
3. Arduino compares strength to set threshold, registers a beat. Activates LED in upper row if beat is high. 

**Every cycle:**

For the last LED row:

1. If all the LEDs are off, ``point_gain = 0``; else, ``point_gain=INSERT_VALUE``
2. If LED row state == button row state, then player score goes up by ``point_gain``; else, score goes down by some set value


