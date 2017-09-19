//print(x);
/*x=x+1;

print(mqtt.isConnected());
mqtt.publish(1,"topic","msg"+x);

print(elapsed);

if(elapsed>1) true;
else false;*/

load = rtData.getLoad();
if(load!=last) {
	print(elapsed+": "+load);
	last = load;
}
