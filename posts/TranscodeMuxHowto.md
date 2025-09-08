**[`ffmpeg`](https://github.com/FFmpeg/FFmpeg) is [FLOSS](https://wikipedia.org/wiki/FLOSS) (no cost), plus is the best tool to transcode/trim/demux/mux. `ffmpeg` is on [_Windows_](https://wikipedia.org/wiki/Windows)&[_Linux_](https://wikipedia.org/wiki/Linux)&[_Android OS_](https://wikipedia.org/wiki/Android_(operating_system))&[_iOS_](https://wikipedia.org/wiki/iOS)&[_macOS_](https://wikipedia.org/wiki/macOS).**

\[This post (which [is also on *SubStack*](https://swudususuwu.substack.com/p/ffmpeg-is-floss-no-cost-mux-tool)) allows [all uses](https://creativecommons.org/licenses/by/2.0/).\]

Tools compatible with this howto:
- For `/bin/sh`: `apt install ffmpeg`
- For _Android OS_: [_FFmpeg Media Encoder_ - Apps on _Google_](https://play.google.com/store/apps/details?id=com.silentlexx.ffmpeggui) (_SilentLexx_) with much success; has a visual interface to `ffmpeg` but can process commands through console.
  - \[Alternative (have not used): [_FFmpeg_ - Apps on _Google_](https://play.google.com/store/apps/details?id=com.crossplat.ffmpegmobile) (_FFmpeg_ from _CrossPlat_).\]
- For misc _Linux_ OSs (suchas [_Ubuntu_](https://ubuntu.com/) or [_Android OS_ +_Termux_](https://play.google.com/store/apps/details?id=com.termux)): `apt install ffmpeg`.
- For _iOS_: [https://shaunhevey.com/posts/how-to-use-ffmpeg-on-ios/](https://shaunhevey.com/posts/how-to-use-ffmpeg-on-ios/).
- For _macOS_: `brew install ffmpeg --with-fdk-aac --with-ffplay --with-freetype --with-libass --with-libvorbis --with-libvpx --with-opus` (or [alternatives](https://superuser.com/questions/624561/install-ffmpeg-on-os-x/624562#624562)).
- For _Windows_: [_FFmpeg_ - Official app in the _Microsoft Store_](https://apps.microsoft.com/detail/9nb2flx7x7wg?hl=en-za&gl=ZA).

# Table of Contents
- [Howto](#howto)
  - [Produce `.mp4`](#produce-mp4)
  - [Produce `.m4a`](#produce-m4a)
    - [Null "video" section muxed to sounds](#null-video-section-muxed-to-sounds)
    - [Concat multiple `.m4a`s](#concat-multiple-m4as)
  - [Mix visuals + sounds into `.mp4`](#mix-visuals-plus-sounds-into-mp4)
  - [Produce `.gif`](#produce-gif)
- [Synopsis](#synopsis)

# Howto
You can use [`../sh/Transcode.sh`](../sh/Transcode.sh) to do all of this for you; what follows is the manual route.
\[*Notice*: versus stock _Android_, have moved default paths `/Music/` to `/Sounds/`, `/Movies/` to `/Visuals/`.\]

\[*Notice*: `.m4a` is used so that sounds are not reencoded if uploaded to platforms (such as *YouTube*) which use the *Advanced Audio Codec*. If the sounds are not to upload to such platforms, use `.opus`, which causes `ffmpeg` to encode to *Opus*.\]

\[*Notice*: if `/storage/emulated/0/` (directory root) is not found, replace with `/sdcard/`.\]

\[*Notice*: Can use examples with _FFmpeg Media Encoder_ or _Termux_ as-is (use absolute paths).\]

## Produce `.mp4`
Example `visuals.mp4` was *4gb*, to compress to *224mb* used:
```sh
nice ffmpeg -i "/storage/emulated/0/Visuals/screen-20240629-045526.mp4" -framerate 30 -c:v libx264 -crf 32 -preset slower "/storage/emulated/0/Visuals/visuals.mp4"
```
. The `libx264` codec compresses visuals best. `-preset slower` instructs it to compress more. You can replace `-crf 32` with `-b:v 2m` to set an exact goal of “compress to *2mbps*”.

\[*Notice*: On some devices, _Android OS_’s permissions require to output to `/storage/emulated/0/Download/`\]

Suppose you want to mux `sounds.mp4` with `visuals.mp4`,

but you want to skip `sounds.mp4`’s *4* second intro, plus limit output to *2* minutes:

## Produce `.m4a`
To demux sounds (into `demux.m4a`); pass `-ss 4` to skip *4* seconds, pass `-t 2:00` to output *2* minutes, pass `-map 0:a:0` (zero-indexed) to demux first input as sounds, pass `-c copy` for instant process (not reencode):
```sh
nice ffmpeg -i "/storage/emulated/0/Download/sounds.mp4 -ss 4 -t 2:00 -map 0:a:0 -c copy "/storage/emulated/0/Sounds/demux.m4a"
```

### Null "video" section muxed to sounds
To mux minimal (all black) "video" section (for programs which require "videos") with an `.m4a`, into `music.mp4`:
`nice ffmpeg -i "/storage/emulated/0/Sounds/demux.m4a" -f lavfi -i color=c=black:s=1920x1080 -c:v libx264 -c:a copy -shortest "/storage/emulated/0/Visuals/music.mp4`


### Concat multiple `.m4a`s
To [concat](https://trac.ffmpeg.org/wiki/Concatenate) the `.m4a` stream (from `visuals.mp4`) with `demux.m4a` (assumes similar codecs used), into `concat.m4a`:
```sh
echo "file '/storage/emulated/0/Visuals/visuals.mp4'" > ./sources
echo "file '/storage/emulated/0/Sounds/demux.m4a'" >> ./sources
nice ffmpeg -f concat -safe 0 -i "sources" -c copy -map 0:a:0 "/storage/emulated/0/Sounds/concat.m4a"
```
- \[*Notice*: `-safe 0` is just for absolute paths\]
- \[*Notice*: replace `-map 0:a:0` with `-map 0:v:0` to concat visuals\]

******

Must "re-encode" for sources with separate codecs used (such as if `demux.m4a` uses [*Advanced Audio Codec*](https://en.wikipedia.org/wiki/Advanced_Audio_Codec) but `visuals.mp4` uses [*Opus*](https://opus-codec.org/comparison/)):
```sh
ffmpeg -i "/storage/emulated/0/Visuals/visuals.mp4" -i "/storage/emulated/0/Sounds/demux.m4a" -filter_complex [0:a][1:a]concat=n=2:v=0:a=1 "/storage/emulated/0/Sounds/concat.m4a"
```
- \[*Notice*: switch `:a` with `:v` to concat visuals\]

## Mix visuals plus sounds into `.mp4`
Now `demux.m4a` is *2* minutes, but `visuals.m4a` is much longer; pass `-stream_loop -1` to mux sounds (as loop) to match `visuals.mp4`, into `mux.mp4`:
```sh
nice ffmpeg -i "/storage/emulated/0/Visuals/visuals.mp4" -stream_loop -1 -i "/storage/emulated/0/Sounds/demux.m4a" -map 0:v:0 -c copy -map 1:a:0 -shortest "/storage/emulated/0/Visuals/mux.mp4"
```
Suppose you want the mix the sounds from `visuals.mp4` with `mux.mp4`, into `mux2.mp4`:
```sh
nice ffmpeg -i "/storage/emulated/0/Visuals/visuals.mp4" -stream_loop -1 -i "/storage/emulated/0/Sounds/demux.m4a" -map 0:a:0 -map 1:a:0 -filter_complex amix=inputs=2:duration=shortest "/storage/emulated/0/Sounds/demux2.m4a"
nice ffmpeg -i "/storage/emulated/0/Visuals/visuals.mp4" -i "/storage/emulated/0/Sounds/demux2.m4a" -map 0:v:0 -c copy -map 1:a:0 -shortest "/storage/emulated/0/Visuals/mux2.mp4"
```
\[*Notice*: `-c copy` is not compatible with `-filter_complex`; unless you want to reencode the visuals (slow), is 2 steps to do this (`demux2.m4a` is an intermediate (temp) file)\]

## Produce `.gif`
Suppose you wish to produce a 10**FPS** *1080p* `.gif` from the first 24 seconds of `visual.mp4`:
```sh
nice ffmpeg -i "/storage/emulated/0/Visuals/visual.mp4" -map 0:v:0 -r 10 -s 1920x1080 -t 24 "/storage/emulated/0/Visuals/visual.gif"
```
or, if you have _ImageMagick_ installed (`apt install magick || apt install imagemagick`):
```sh
nice ffmpeg -i "/storage/emulated/0/Visuals/visual.mp4" -map 0:v:0 -r 10 -s 1920x1080 -t 24 -f image2pipe -vcodec ppm - | convert -delay $(expr 100 / 10) - "/storage/emulated/0/Visuals/visual.gif"
```
will use more disk but has dither and palette improved.
Optimization (lossless compression, such as: duplicate frames and duplicate palettes are reduced) through _Gifsicle_ (`apt install gifsicle`):
```sh
nice gifsicle -O2 "/storage/emulated/0/Visuals/visual.gif" --batch
```

# Synopsis
Lists of commands&options which `ffmpeg` can use:
- [ffmpeg Documentation](https://ffmpeg.org/ffmpeg.html)
- [complete list of `ffmpeg` flags / commands](https://gist.github.com/tayvano/6e2d456a9897f55025e25035478a3a50#file-gistfile1-txt)

How to use extra tools (which `ffmpeg`'s _GPLv2_ version has):
[https://github.com/FFmpeg/FFmpeg/blob/master/LICENSE.md](https://github.com/FFmpeg/FFmpeg/blob/master/LICENSE.md)

`mux.mp4`/`mux2.mp4` syntax was used to produce:
- [*BUD*, *Arduino Parkour* v0.6.24.62, production + walkthrough + hoverboard tour](https://www.youtube.com/watch?v=3L8tskPqdsM)
- [Sakura School Simulator, howto import+use props (robot shop class), Swudu Susuwu](https://www.youtube.com/watch?v=YFgRW58mbG4)

`visual.gif` syntax was used to produce:
- <https://www.deviantart.com/swudususuwu/art/gif-BUD-Arduino-Parkour-v0-6-24-62-walkthrough-1233444178>
- <https://www.deviantart.com/swudususuwu/art/Sakura-School-Simulator-howto-use-robot-props-loop-1019774750>

******

[_Video Transcoder_ - Apps on _Google_](https://play.google.com/store/apps/details?id=protect.videoeditor) (a visual interface to `ffmpeg`) was cool versus most “_video editor_” apps -- but is not available for new versions of _Android OS_, can not loop (just has trim + convert (which can act as demux) + resize + compress), is slow (can not pass `-c copy` to `ffmpeg`, thus always reincodes inputs.)

