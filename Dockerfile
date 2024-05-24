FROM gcc
WORKDIR /custom
COPY * /custom/
RUN ["apt", "update"]
RUN ["apt", "install", "g++", "-y"]
RUN ["chmod", "+x", "./compile.sh"]
RUN ./compile.sh