FROM imaginex786/openvino:baseimage
RUN apt -y install git && \
    mkdir -p /opt/ &&\
    cd /opt/ &&\
    git clone --depth 1 https://github.com/gabime/spdlog.git &&\
    cd spdlog &&\
    mkdir -p build &&\
    cd build &&\
    cmake .. &&\
    make -j$(nproc) &&\
    make install

# installing yaml-cpp to parse config
RUN cd /opt/ &&\
    git clone --depth 1 https://github.com/jbeder/yaml-cpp.git &&\
    cd yaml-cpp &&\
    mkdir -p build &&\
    cd build &&\
    cmake -DYAML_CPP_BUILD_TESTS=OFF .. &&\
    make -j$(nproc) &&\
    make install
COPY . /workspace
RUN cd /workspace && \
    mkdir build && \
    cd build && \
    /bin/bash -c "source /opt/intel/openvino/bin/setupvars.sh && \
    cmake .. && \
    make -j$(nproc)"

WORKDIR /workspace