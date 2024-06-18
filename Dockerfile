FROM ubuntu:latest

WORKDIR /app

RUN apt update
RUN apt install -y wget
RUN wget https://hypersomnia.xyz/builds/latest/Hypersomnia-Headless.AppImage
RUN chmod +x Hypersomnia-Headless.AppImage

RUN mkdir /root/.config
RUN mkdir /root/.config/Hypersomnia
RUN mkdir /root/.config/Hypersomnia/user
COPY cmake/dockerfile_server_config.lua /root/.config/Hypersomnia/user/config.force.lua

CMD ["./Hypersomnia-Headless.AppImage", "--appimage-extract-and-run"]

EXPOSE 8412/udp
EXPOSE 9000-9010/udp

