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
last=0;


interval=100;

//setup mqtt client (for publishing display information)
mqtt_host = "localhost";
recorder_path = "/tmp/blub.tcx";
CP = 300;
erg_path = "/tmp/test.erg";
