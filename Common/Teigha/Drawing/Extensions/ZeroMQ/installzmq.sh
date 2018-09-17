cd ~/Downloads
wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.3.tar.gz
tar -xzf ./libsodium-1.0.3.tar.gz
cd ./libsodium-1.0.3
./configure --disable-pie
sudo make install

cd ..
wget http://download.zeromq.org/zeromq-4.1.3.tar.gz
tar -xzf ./zeromq-4.1.3.tar.gz
cd ./zeromq-4.1.3
./configure CXXFLAGS="-fPIC"
sudo make install
