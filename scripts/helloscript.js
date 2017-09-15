/*device.start();
device.setup();

//data = device.getRealtimeData();
print(rtData.getWatts());
print(rtData.getLoad());

device.setLoad(150);*/

device.foundDevice.connect(function(channel, device_number, device_id) {
	print("foundDevice = "+channel+", "+device_number+", "+device_id);
});

device.searchComplete.connect(function(channel) {
	print("searchComplete = "+channel);
});

device.signalStrength.connect(function(channel, reliability) {
	print("signalStrength = "+channel+": "+reliability);
});
x=0;
interval=100;
