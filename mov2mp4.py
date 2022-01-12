from converter import Converter

conv = Converter()

info = conv.probe('/home/kevlu8/Desktop/thing/IMG_3156.MOV')

convert = conv.convert('/home/kevlu8/Desktop/thing/IMG_3156.MOV', '/home/kevlu8/video.mp4', {
    'format': 'mp4',
    'audio': {
        'codec': 'aac',
        'samplerate': 11025,
        'channels': 2
    },
    'video': {
        'codec': 'hevc',
        'width': 1920,
        'height': 1080,
        'fps': 30
    }})

for timecode in convert:
    print(f'\rConverting ({timecode:.2f}) ...')