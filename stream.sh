#ffmpeg (no worky)
#/opt/vc/bin/raspivid --timeout 0 -w 960 -h 540 -b 500000 -o - | ffmpeg -i - -vcodec copy -f mpegts udp://192.168.0.104:8080

#gstreamer (no worky)
#/opt/vc/bin/raspivid --timeout 0 -w 960 -h 540 -b 500000 -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay ! gdppay ! tcpserversink host=192.168.0.104 port=8080
#/opt/vc/bin/raspivid -t 0 -h 540 -w 960 -fps 10 -hf -b 200000 -o - | gst-launch-1.0 -v fdsrc ! h264parse !  rtph264pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=192.168.0.116 port=8554

#vlc almost worked
#/opt/vc/bin/raspivid --timeout 0 -w 960 -h 540 -b 500000 -o - | vlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ogg,dst=192.168.0.116:8554/stream}' :demux=h264
cvlc v4l2:///dev/video0 --v4l2-width 320 --v4l2-height 240 --v4l2-fps 5 --v4l2-chroma h264 --sout '#standard{access=http,mux=ts,dst=192.168.0.116:8554/stream}'
#cvlc v4l2:///dev/video0 --v4l2-width 320 --v4l2-height 240 --v4l2-fps 1 --v4l2-chroma h264 --sout '#transcode{vcodec=mp4v,vb=3000}:standard{access=http,mux=ts,dst=192.168.0.116:8554/stream}'
