FROM asymworks/multiarch-alpine:${TARGETARCH}${TARGETVARIANT}-latest-stable AS build
RUN apk add --no-cache alpine-sdk gcc linux-headers libxml2-dev cmake autoconf automake libusb-dev bash
RUN git clone https://github.com/wmbusmeters/wmbusmeters.git && \
    git clone https://github.com/weetmuts/rtl-wmbus.git && \
    git clone https://github.com/merbanan/rtl_433.git && \
    git clone https://github.com/osmocom/rtl-sdr.git
WORKDIR /rtl-sdr
RUN autoreconf -i && ./configure
RUN make && make install
WORKDIR /wmbusmeters
RUN make
WORKDIR /rtl-wmbus
RUN make release && chmod 755 build/rtl_wmbus
WORKDIR /rtl_433
RUN mkdir build && cd build && cmake ../ && make


FROM asymworks/multiarch-alpine:${TARGETARCH}${TARGETVARIANT}-latest-stable as scratch
ENV QEMU_EXECVE=1
RUN apk add --no-cache mosquitto-clients libstdc++ curl libusb rtl-sdr libxml2 netcat-openbsd
WORKDIR /wmbusmeters
COPY --from=build /wmbusmeters/build/wmbusmeters /wmbusmeters/wmbusmeters
COPY --from=build /rtl-wmbus/build/rtl_wmbus /usr/bin/rtl_wmbus
COPY --from=build /rtl_433/build/src/rtl_433 /usr/bin/rtl_433
COPY --from=build /wmbusmeters/docker/docker-entrypoint.sh /wmbusmeters/docker-entrypoint.sh
COPY --from=build /usr/local/bin/* /usr/local/bin/
COPY --from=build /usr/local/lib/librtlsdr* /usr/local/lib/
COPY --from=build /etc/udev/rules.d/rtl-sdr.rules /etc/udev/rules.d/rtl-sdr.rules
RUN mkdir -p /etc/modprobe.d && \
    echo 'blacklist dvb_usb_rtl128xxu' >> /etc/modprobe.d/blacklist.conf
VOLUME /wmbusmeters_data/
CMD ["sh", "/wmbusmeters/docker-entrypoint.sh"]
