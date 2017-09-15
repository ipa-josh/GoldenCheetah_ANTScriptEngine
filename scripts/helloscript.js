device.start();
device.setup();

//data = device.getRealtimeData();
print(rtData.getWatts());
print(rtData.getLoad());

device.setLoad(150);
