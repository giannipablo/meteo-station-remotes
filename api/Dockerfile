FROM ubuntu:latest

WORKDIR /usr/src/app

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=America/New_York

RUN apt-get update && apt-get install -y locales tzdata python3.9 python3-pip && rm -rf /var/lib/apt/lists/* && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8
ENV LANG en_US.utf8

COPY main.py /usr/src/app/
COPY requirements.txt /usr/src/app/

RUN pip install -U --no-cache-dir pip
RUN pip install --no-cache-dir -r requirements.txt

CMD gunicorn main:app --bind=0.0.0.0:8000 -w 4 -k uvicorn.workers.UvicornWorker
