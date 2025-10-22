#!/bin/sh
#
# /* Attribution (henceforth "*this attribution*", whose syntax is *Markdown*): 2024 [Swudu Susuwu](https://swudususuwu.substack.com)
#  * <https://github.com/SwuduSusuwu/SusuLib/> has the newest version of `./sh/Transcode.sh` (henceforth "*this source code*").
#  * If *this attribution* is shown, *this source code* allows all uses. *This attribution* constitutes the most permissive which is compatible with [*GPLv2*](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html) + [*Apache 2*](https://www.apache.org/licenses/LICENSE-2.0.html), which is suitable for personal use (also suitable for school use). Just source code (or executables) must reproduce **this attribution**.
#  * If *this attribution* is not professional enough for business use: businesses can use *this source code* through included versions of [*GPLv2*](./LICENSE_GPLv2), [*Apache 2*](./LICENSE), or through both of those. */
# /* This is used to {mux (merge), demux (split), compress (reincode), transcode (convert)} formats such as {`.mp4`, `webm`, `.opus`}. */

# Start of user configurable variables
# Set for relative dirs / paths
TRANSCODE_ROOT="/sdcard/" #"/sdcard/" for Android or AOSP. "/" for most Unices.
TRANSCODE_IN_VISUALS="" #"${TRANSCODE_ROOT}/Movies/" for Android or AOSP. "~/" for most Unices.
TRANSCODE_IN_SOUNDS="" #${TRANSCODE_ROOT}Sounds/" #"${TRANSCODE_ROOT}/Music/" for Android or AOSP. "~/" for most Unices.
TRANSCODE_SOUND_DEFAULT="${TRANSCODE_ROOT}/Sounds/Susuwu/Swudu Susuwu's organs 2024.02.16 1242.mp3" #Default background music (if not passed `${2}`).
TRANSCODE_OUT_VISUALS="" #${TRANSCODE_ROOT}Visuals/" #"${TRANSCODE_ROOT}/Movies/processed/" for Android or AOSP. "~/processed/" for most Unices.
TRANSCODE_OUT_GIF="" #${TRANSCODE_ROOT}Photos/" #"${TRANSCODE_ROOT}/Pictures/processed/" for Android or AOSP. "~/processed/" for most Unices.
# Flags / options
TRANSCODE_MISC_FLAGS="-hide_banner" #Flags for all modes
TRANSCODE_MP4_ENABLED="true" #Produce "{1%.*}${TRANSCODE_MP4_CONTAINER}"
TRANSCODE_MP4_CONTAINER=".out.mp4" #Best for YouTube and personal use.
TRANSCODE_MP4_SKIP="00:00:00.000" #Seconds to skip from start of input.
TRANSCODE_MP4_DURATION="" #Count of seconds to output, or "" for auto.
TRANSCODE_MP4_CODEC="libx264" #MPEG-4 Part 10, Advanced Video Coding (MPEG-4 AVC). Best for YouTube and personal use.
TRANSCODE_MP4_PROFILE="high" #{"baseline", "main", "high"}; ["high" is best for YouTube.](https://superuser.com/questions/489087/what-are-the-differences-between-h-264-profiles)
TRANSCODE_MP4_BF="2" #max consecutive B-frames. Default is "16", YouTube uses "2".
TRANSCODE_MP4_FPS="30" #30 (or 60, if source is >=60) is best for YouTube.
TRANSCODE_MP4_G_FACTOR="2" #FPS*2 = 2 second groups (best for YouTube).
TRANSCODE_MP4_RESOLUTION="" #{"2400x1080", "1200x540", "600x270"} #producees interpolation artifacts, unless source resolution is multiple of output resolution #Replace `<width>x` with `-1:`, to autoscale (since `2400x1080` and `1920x1080` are both common).
TRANSCODE_MP4_PRESET="slow" #Quality; {"ultrafast", "superfast", "veryslow", "faster", "fast", "medium", "slow", "slower", "veryslow"}. "veryslow" uses the most resources to encode, but produces (small) output most close to original.
TRANSCODE_MP4_MOVFLAGS="+faststart" #Best for YouTube; moves metadata to start (allows partial download).
#TRANSCODE_MP4_BITS="-b:v 10m" #Best for YouTube? Not sure if this is used with `-crf`.
#TRANSCODE_MP4_THREADS="-threads 4" #`ffmpeg`'s default is to use all cores for `.gif`. If it does not for `.mp4`, it must have reasons not to.
#TRANSCODE_MP4_CPUUSED="-cpu-used 0" #["Most CPU intensive, but best fastest. Uses all cores."](https://scribbleghost.net/2018/10/26/recommended-encoding-settings-for-youtube-in-ffmpeg/) #Not sure how this differs from `-threads`.
TRANSCODE_MP4_CRF="32" #Amount of compression; https://scribbleghost.net/2018/10/26/recommended-encoding-settings-for-youtube-in-ffmpeg/ says YouTube is best with "18", but computer graphics is more compressible. #"22" uses 3316kilobit/s, "27" uses 1985/s, "32" uses 1222/s
TRANSCODE_MP4_PIX_FMT="" #yuv420p" #Subsample to 4:2:0 (chroma 1/4, luma whole); best for YouTube, but now gives "Error initializing the muxer for yuv420p: Invalid argument".
TRANSCODE_MP4_SOUND_ENABLED="true" #Produce "$(basename "${TRANSCODE_SOUND_IN}") {1%.*}${TRANSCODE_MP4_CONTAINER}"
TRANSCODE_SOUND_FORMAT="234" #`yt-dlp` format for `TRANSCODE_SOUND_DEFAULT`. "234" has best sound, "233" is smaller
TRANSCODE_SOUND_CODEC_COMMAND="-c:a aac -profile:a aac_low -ac 2" #Advanced Audio Codec - Low Complexity (AAC-LC) with 2 channels; best for YouTube. # -filter_complex \"[1:0] apad\"
#TRANSCODE_SOUND_SAMPLES="-ar 48000 -ac 2 -b:a" #If source is lossless, use "96000".
#TRANSCODE_SOUND_BITS="-b:a 128k" #If source is lossless, use "320k".
TRANSCODE_GIF_ENABLED="true" #Produce "{1%.*}.gif"
TRANSCODE_GIF_CONTAINER=".out.gif" #Best for YouTube and personal use.
TRANSCODE_GIF_PIX_FMT="rgb8" #TODO; figure out what happened to "rgb24". Produce per-video palette (or per-frame palettes). "" for auto.
TRANSCODE_GIF_SKIP="00:00:00.000" #${TRANSCODE_MP4_SKIP}" #Seconds to skip from start of input.
TRANSCODE_GIF_DURATION="" #${TRANSCODE_MP4_DURATION}" #Count of seconds to output, or "" for auto.
TRANSCODE_GIF_FPS="10" #Suits most uses. Slower looks worse, higher increases filesize.
#TRANSCODE_GIF_FILTER_FPS="${TRANSCODE_GIF_FPS:+"fps=${TRANSCODE_GIF_FPS};"}"
#TRANSCODE_GIF_COMPLEX="-filter_complex \"${TRANSCODE_GIF_FILTER_FPS}scale=500:-1:flags=lanczos,split[v1][v2]; [v1]palettegen=stats_mode=full [palette];[v2]palette]paletteuse=dither=sierra2_4a\"" #Palettes&dithering improved
#TRANSCODE_GIF_RESOLUTION="600x270" #{"2400x1080", "1200x540", "600x270"} #producees interpolation artifacts, unless source resolution is multiple of output resolution #Replace `<width>x` with `-1:`, to autoscale (since `2400x1080` and `1920x1080` are both common).
TRANSCODE_GIF_RESOLUTION="-1:270" #{"2400x1080", "1200x540", "600x270"} #producees interpolation artifacts, unless source resolution is multiple of output resolution #Replace `<width>x` with `-1:`, to autoscale (since `2400x1080` and `1920x1080` are both common).
TRANSCODE_USE_GIFSICLE="true" #Compression improved
TRANSCODE_GIFSICLE_BATCH="true" #`--batch` (overwrite original).
TRANSCODE_GIFSICLE_CONTAINER=".gifsicle" #Unused with `--batch`
TRANSCODE_USE_IMAGEMAGICK="true" #Increases size and encode time, but dither (and color palette) improved
TRANSCODE_IMAGEMAGICK_CONTAINER=".magick" #Suffix which marks `magick` versions.
if ${TRANSCODE_USE_IMAGEMAGICK}; then
	TRANSCODE_GIF_CONTAINER="${TRANSCODE_IMAGEMAGICK_CONTAINER}${TRANSCODE_GIF_CONTAINER}"
fi
# End of user configurable variables

TRANSCODE_VISUAL_IN="${TRANSCODE_IN_VISUALS}${1}"
if [ -n "${2}" ]; then
	TRANSCODE_SOUND_IN="${TRANSCODE_IN_SOUNDS}${2}"
else
	TRANSCODE_SOUND_IN="${TRANSCODE_SOUND_DEFAULT}"
	echo "Sound path defaults to \"${TRANSCODE_SOUND_IN}\"."
fi
VISUAL_IN_EXTENSIONLESS() ( echo "${TRANSCODE_VISUAL_IN%.*}" )
SOUND_IN_EXTENSIONLESS() ( echo "${TRANSCODE_SOUND_IN%.*}" )
if [ -n "${TRANSCODE_MP4_RESOLUTION%x*}" ]; then
	TRANSCODE_MP4_RESOLUTION_COMMAND="-s ${TRANSCODE_MP4_RESOLUTION}"
elif [ -n "${TRANSCODE_MP4_RESOLUTION%:*}" ]; then
	TRANSCODE_MP4_RESOLUTION_COMMAND="-vf scale=${TRANSCODE_MP4_RESOLUTION}"
else
	TRANSCODE_MP4_RESOLUTION_COMMAND=""
fi
if [ -n "${TRANSCODE_GIF_RESOLUTION%:*}" ]; then
	TRANSCODE_GIF_RESOLUTION_COMMAND="-vf scale=${TRANSCODE_GIF_RESOLUTION}"
else
	TRANSCODE_GIF_RESOLUTION_COMMAND="-s ${TRANSCODE_GIF_RESOLUTION}"
fi
TRANSCODE_MP4_OUT="$(VISUAL_IN_EXTENSIONLESS)${TRANSCODE_MP4_CONTAINER}"
TRANSCODE_MP4_SOUND_OUT_PATH="$(VISUAL_IN_EXTENSIONLESS)_$(basename "$(SOUND_IN_EXTENSIONLESS)")${TRANSCODE_MP4_CONTAINER}"
if [ -z "${1}" ] || [ "-h" = "${1}" ] || [ "--help" = "${1}" ]; then
	echo "Usage: ${0} \"<path-to-input-visuals>\" \"[<optional-path-to-input-sounds>]\""
	return 1
fi

TRANSCODE_MP4_OUT_PATH="${TRANSCODE_OUT_VISUALS}${TRANSCODE_MP4_OUT}"
if ${TRANSCODE_MP4_ENABLED}; then
	TRANSCODE_GIF_PIX_FMT_COMMAND="${TRANSCODE_GIF_PIX_FMT:+"-pix_fmt ${TRANSCODE_GIF_PIX_FMT}"}"
	TRANSCODE_MP4_DURATION_COMMAND="${TRANSCODE_MP4_DURATION:+"-t ${TRANSCODE_MP4_DURATION}"}"
	TRANSCODE_MP4_FPS_COMMAND="${TRANSCODE_MP4_FPS:+"-r ${TRANSCODE_MP4_FPS} -g $((TRANSCODE_MP4_FPS * TRANSCODE_MP4_G_FACTOR))"}"
	TRANSCODE_MP4_CODEC_COMMAND="${TRANSCODE_MP4_CODEC:+"-c:v ${TRANSCODE_MP4_CODEC}"}"
	TRANSCODE_MP4_CRF_COMMAND="${TRANSCODE_MP4_CRF:+"-crf ${TRANSCODE_MP4_CRF}"}"
	TRANSCODE_MP4_PRESET_COMMAND="${TRANSCODE_MP4_PRESET:+"-preset ${TRANSCODE_MP4_PRESET}"}"
	TRANSCODE_MP4_MOVFLAGS_COMMAND="${TRANSCODE_MP4_MOVFLAGS:+"-movflags ${TRANSCODE_MP4_MOVFLAGS}"}"
	export TRANSCODE_MP4_PROFILE_COMMAND="${TRANSCODE_MP4_PROFILE:+"-profile:v ${TRANSCODE_MP4_PROFILE}"}"
	export TRANSCODE_MP4_BF_COMMAND="${TRANSCODE_MP4_BF:+"-bf ${TRANSCODE_MP4_BF}"}"
	if [ -e "${TRANSCODE_MP4_OUT_PATH}" ]; then
		echo "$0: Notice: \"${TRANSCODE_MP4_OUT_PATH}\" exists, execute \`rm \"${TRANSCODE_MP4_OUT_PATH}\"\` to redo."
	else
#shellcheck disable=SC2086 #Those `_COMMAND`s are supposed to expand into arguments.
		nice ffmpeg ${TRANSCODE_MISC_FLAGS} -i "${TRANSCODE_VISUAL_IN}" ${TRANSCODE_MP4_FPS_COMMAND} ${TRANSCODE_MP4_RESOLUTION_COMMAND} ${TRANSCODE_MP4_CODEC_COMMAND} ${TRANSCODE_MP4_CRF_COMMAND} ${TRANSCODE_MP4_PIX_FMT} ${TRANSCODE_MP4_PROFILE_COMMAND} ${TRANSCODE_MP4_BF_COMMAND} ${TRANSCODE_MP4_PRESET_COMMAND} ${TRANSCODE_MP4_MOVFLAGS_COMMAND} -ss ${TRANSCODE_MP4_SKIP} ${TRANSCODE_MP4_DURATION_COMMAND} "${TRANSCODE_MP4_OUT_PATH}"
	fi
fi

if ${TRANSCODE_MP4_ENABLED} && ${TRANSCODE_MP4_SOUND_ENABLED} && [ -e "${TRANSCODE_MP4_OUT_PATH}" ]; then
	if [ -e "${TRANSCODE_MP4_SOUND_OUT_PATH}" ]; then
		echo "$0: Notice: \"${TRANSCODE_MP4_SOUND_OUT_PATH}\" exists, execute \`rm \"${TRANSCODE_MP4_SOUND_OUT_PATH}\"\` to redo."
	else
		if [ ! -e "${TRANSCODE_SOUND_IN}" ] && command -v "yt-dlp"; then
			TRANSCODE_DEFAULT_BACKGROUND_MUSIC="https://www.youtube.com/watch?v=jbyE0W4FFjAq"
			yt-dlp -f ${TRANSCODE_SOUND_FORMAT} "${TRANSCODE_DEFAULT_BACKGROUND_MUSIC}" -o "${TRANSCODE_SOUND_IN}" || {
				echo "$0: Error: \`yt-dlp\` failed to download default background music. Use some other program to download \"${TRANSCODE_DEFAULT_BACKGROUND_MUSIC}\" to \"${TRANSCODE_SOUND_IN}\"."
			}
#			mv "Swudo\ Susuwu\'s\ organs\ 2024⧸02⧸16\ 12_42\,\ amateur\,\ Creative\ Commons\,\ allows\ all\ uses\ \[jbyE0W4FFjA\].mp4" "${TRANSCODE_SOUND_IN}"
		fi
#shellcheck disable=SC2086 #Those `_COMMAND`s are supposed to expand into arguments.
		nice ffmpeg ${TRANSCODE_MISC_FLAGS} -i "${TRANSCODE_MP4_OUT_PATH}" -stream_loop -1 -i "${TRANSCODE_SOUND_IN}" -map 0:v:0 -c copy -map 1:a:0 -shortest ${TRANSCODE_SOUND_CODEC_COMMAND} "${TRANSCODE_MP4_SOUND_OUT_PATH}"
#		nice ffmpeg ${TRANSCODE_MISC_FLAGS} -i "${TRANSCODE_MP4_OUT_PATH}" -i "${TRANSCODE_SOUND_IN}" -map 0:v:0 -c copy -map 1:a:0 ${TRANSCODE_SOUND_CODEC_COMMAND} "${TRANSCODE_MP4_SOUND_OUT_PATH}" #`-af "anull"`
	fi
fi

TRANSCODE_GIF_OUT_PATH="${TRANSCODE_OUT_GIF}$(VISUAL_IN_EXTENSIONLESS)${TRANSCODE_GIF_CONTAINER}"
if ${TRANSCODE_GIF_ENABLED}; then
	export TRANSCODE_GIF_PIX_FMT_COMMAND="${TRANSCODE_GIF_PIX_FMT:+"-pix_fmt ${TRANSCODE_GIF_PIX_FMT}"}"
	TRANSCODE_GIF_DURATION_COMMAND="${TRANSCODE_GIF_DURATION:+"-t ${TRANSCODE_GIF_DURATION}"}"
	TRANSCODE_GIF_FPS_COMMAND="${TRANSCODE_GIF_FPS:+"-r ${TRANSCODE_GIF_FPS}"}"
	if [ -e "${TRANSCODE_GIF_OUT_PATH}" ]; then
		echo "$0: Notice: \"${TRANSCODE_GIF_OUT_PATH}\" exists, execute \`rm \"${TRANSCODE_GIF_OUT_PATH}\"\` to redo."
	else
#shellcheck disable=SC2086 #Those `_COMMAND`s are supposed to expand into arguments.
		if ${TRANSCODE_USE_IMAGEMAGICK}; then
#shellcheck disable=SC2046 #Can't quote variables with arguments.
			nice ffmpeg ${TRANSCODE_MISC_FLAGS} -i "${TRANSCODE_VISUAL_IN}" -map 0:v:0 ${TRANSCODE_GIF_FPS_COMMAND} ${TRANSCODE_GIF_RESOLUTION_COMMAND} -ss "${TRANSCODE_GIF_SKIP}" ${TRANSCODE_GIF_DURATION_COMMAND} -f image2pipe -vcodec ppm - | convert -delay $((100 / TRANSCODE_GIF_FPS)) - "${TRANSCODE_GIF_OUT_PATH}"
			command -v "convert" || echo "${0}: Error: \`convert\` not found. Execute \`pkg install imagemagick\`"
		else
			nice ffmpeg ${TRANSCODE_MISC_FLAGS} -i "${TRANSCODE_VISUAL_IN}" -map 0:v:0 ${TRANSCODE_GIF_COMPLEX} ${TRANSCODE_GIF_PIX_FMT_COMMAND} ${TRANSCODE_GIF_FPS_COMMAND} ${TRANSCODE_GIF_RESOLUTION_COMMAND} -ss "${TRANSCODE_GIF_SKIP}" ${TRANSCODE_GIF_DURATION_COMMAND} "${TRANSCODE_GIF_OUT_PATH}"
#			nice ffmpeg ${TRANSCODE_MISC_FLAGS} -i "${TRANSCODE_VISUAL_IN}" -filter_complex "${TRANSCODE_GIF_FILTER_FPS}scale=500:-1:flags=lanczos,split[v1][v2]; [v1]palettegen=stats_mode=full [palette];[v2]palette]paletteuse=dither=sierra2_4a" -map 0:v:0 ${TRANSCODE_GIF_PIX_FMT_COMMAND} ${TRANSCODE_GIF_FPS_COMMAND} ${TRANSCODE_GIF_RESOLUTION_COMMAND} -ss ${TRANSCODE_GIF_SKIP} ${TRANSCODE_GIF_DURATION_COMMAND} "${TRANSCODE_GIF_OUT_PATH}"
		fi
		if ${TRANSCODE_USE_GIFSICLE}; then
#			TRANSCODE_GIFSICLE_COMMAND="nice gifsicle -O2 \"${TRANSCODE_GIF_OUT_PATH}\""
			if ${TRANSCODE_GIFSICLE_BATCH}; then
				nice gifsicle -O2 --batch "${TRANSCODE_GIF_OUT_PATH}"
#				"${TRANSCODE_GIFSICLE_COMMAND} --batch"
			else
				nice gifsicle -O2 "${TRANSCODE_GIF_OUT_PATH}" -o "${TRANSCODE_GIF_OUT_PATH%"${TRANSCODE_GIF_CONTAINER}"}${TRANSCODE_GIFSICLE_CONTAINER}${TRANSCODE_GIF_CONTAINER}"
#				"${TRANSCODE_GIFSICLE_COMMAND} -o \"${TRANSCODE_GIF_OUT_PATH%"${TRANSCODE_GIF_CONTAINER}"}${TRANSCODE_GIFSICLE_CONTAINER}${TRANSCODE_GIF_CONTAINER}\""
			fi
			command -v "gifsicle" || echo "${0}: Error: \`gifsicle\` not found. Execute \`pkg install gifsicle\`"
		fi
	fi
fi
return 0

#nice ffmpeg ${TRANSCODE_MISC_FLAGS} -i "${TRANSCODE_IN_SOUNDS}Susuwu/Swudo Susuwu's organs 2024.02.16 1242ss431t932.ogg" -stream_loop  -1 -i "${TRANSCODE_IN_SOUNDS}nu/UU! NYA! [GHbPK0Ymjw0].m4a" -filter_complex amix=inputs=2:duration=shortest "${TRANSCODE_IN_SOUNDS}Susuwu/Swudo Susuwu's organs 2024.02.16 1242ss431t932+UuNya.opus"

