FROM arm64v8/ubuntu

RUN apt update
RUN apt install -y git
RUN apt install -y software-properties-common
RUN apt install -y ca-certificates
RUN apt install -y wget
RUN apt install -y curl
RUN apt install -y ssh
RUN apt install -y sudo
RUN apt install -y build-essential
RUN apt install -y python3 python3-pip
RUN curl --proto '=https' --tlsv1.2 -sSf https://just.systems/install.sh | bash -s -- --to /usr/bin