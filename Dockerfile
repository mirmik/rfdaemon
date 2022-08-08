FROM positioner-env:amd64

ADD . /root/rfdaemon
WORKDIR /root/rfdaemon
RUN ./make.py
RUN sudo ./make.py install

CMD rfdaemon --noconsole