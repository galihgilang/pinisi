package org.opencv.samples.colorblobdetect;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.annotation.TargetApi;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Build;
import android.util.Log;

public class BluetoothConnection {
	private	BluetoothAdapter	ba;
	private	BluetoothSocket		bs;
	private BluetoothDevice		bd;
	//private InputStream			is;
	private OutputStream		os;
	public	String				readmessage;
	private	String				TAG;
	
	// Message types sent from the BluetoothChatService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
	
	public BluetoothConnection() {
		// TODO Auto-generated constructor stub
		TAG = "BluetoothConnection";
		ba = BluetoothAdapter.getDefaultAdapter();
		ba.getState();
	}
	
	public void BluetoothOn() {
		ba.enable();
	}
	
	public void BluetoothOff() {
		ba.disable();
	}
	
	public boolean IsBluetoothOn() {
		return ba.isEnabled();
	}
	
	@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
	public boolean IsConnected() {
		if(bs != null)return bs.isConnected();
		else return false;
	}
	
	public String getdevicename(){
		return bd.getName();
	}
	 
	public boolean setdevice(){
    	Set<BluetoothDevice> pairedDevices = ba.getBondedDevices();
        if(pairedDevices.size() > 0){
            for(BluetoothDevice device : pairedDevices){
                
                if(device.getName().equalsIgnoreCase("Bluetooth_V3")){
                	bd = device;
                }
            }
        }
        return bd!=null;
    };
    
    @TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
	void sambung() {
    	OutputStream tmpdataout = null;
        
        if(bd!= null){
        	try {
        		bs = bd.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"));
        		//socket = device.createRfcommSocketToServiceRecord(UUID.randomUUID());
        		bs.connect();
        		tmpdataout = bs.getOutputStream();
        		//flagconect = true;
        	} 
        	catch (IOException e) { 
        	
        	}
        }
        //is = tmpdatain;
        os = tmpdataout;
        
        if(bs.isConnected()){
        	ConnectedThread a = new ConnectedThread(bs);
        	a.start();
        }
    };
    
    public void send(String s){
    	try {
			os.write(s.getBytes());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			//e.printStackTrace();
			Log.e(TAG, e.getMessage());
		}
    }
    
    /*private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case MESSAGE_READ:
                byte[] readBuf = (byte[]) msg.obj;
                // construct a string from the valid bytes in the buffer
                String readMessage = new String(readBuf, 0, msg.arg1);
                break;
            }
        }
    };*/
    
    private class ConnectedThread extends Thread {
	    private final BluetoothSocket mmSocket;
	    private final InputStream mmInStream;
	    public ConnectedThread(BluetoothSocket socket) {
	        mmSocket = socket;
	        InputStream tmpIn = null;
	        // Get the input and output streams, using temp objects because
	        // member streams are final
	        try {
	            tmpIn = mmSocket.getInputStream();
	        } catch (IOException e) { }
	 
	        mmInStream = tmpIn;
	    }
	 
	    @TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
		public void run() {
	        byte[] buffer = new byte[1024];  // buffer store for the stream
	        int bytes; // bytes returned from read()
	 
	        // Keep listening to the InputStream until an exception occurs
	        while (mmSocket.isConnected()) {
	            try {
	                // Read from the InputStream
	                bytes = mmInStream.read(buffer);
	                readmessage = new String(buffer, 0, bytes);
	                // Send the obtained bytes to the UI activity
	                //mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer).sendToTarget();
	            } catch (IOException e) {
	                //break;
	            	Log.e(TAG, e.getMessage());
	            }
	        }
	    }
	}
	
	
}
