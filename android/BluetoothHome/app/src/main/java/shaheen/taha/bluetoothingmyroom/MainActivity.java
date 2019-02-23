package shaheen.taha.bluetoothingmyroom;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.IOException;
import java.util.UUID;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button energySaverButton = (Button) findViewById(R.id.button1);
        final Button fanButton = (Button) findViewById(R.id.button2);
        final Button switchBoard2Button = (Button) findViewById(R.id.button3);
        final Button socketButton = (Button) findViewById(R.id.button4);

        energySaverButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendData("1t5t");
                updaterFunction();
            }
        });

        fanButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendData("2t5t");
                updaterFunction();
            }
        });

        switchBoard2Button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendData("3t5t");
                updaterFunction();
            }
        });

        socketButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendData("4t5t");
                updaterFunction();

            }
        });
    }

    private void updaterFunction(){
        final Button energySaverButton = (Button) findViewById(R.id.button1);
        final Button fanButton = (Button) findViewById(R.id.button2);
        final Button switchBoard2Button = (Button) findViewById(R.id.button3);
        final Button socketButton = (Button) findViewById(R.id.button4);
        String string = receiveData();
        if(string.startsWith("t1111")){
            String data = string.substring(5,9);
            Log.d(TAG, "receiving data:"+ data);
            if(data.charAt(0)=='1'){
                energySaverButton.setText("Energy Saver ON");
            }
            else {
                energySaverButton.setText("Energy Saver OFF");
            }
            if(data.charAt(1)=='1'){
                fanButton.setText("Fan ON");
            }
            else {
                fanButton.setText("Fan OFF");
            }
            if(data.charAt(2)=='1'){
                switchBoard2Button.setText("Switch Board 2 ON");
            }
            else {
                switchBoard2Button.setText("Switch Board 2 OFF");
            }
            if(data.charAt(3)=='1'){
                socketButton.setText("Socket ON");
            }
            else {
                socketButton.setText("Socket OFF");
            }
        }
    }

    @Override
    public void onStart() {
        super.onStart();
        new ConnectBT().execute();
    }

    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    //SPP UUID. Look for it
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    //private static String address = "98:D3:31:FB:61:26";
    private static String address = "00:18:E4:34:EF:E1";
    private static String TAG = "fromPhone";


    private class ConnectBT extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute() {
            // progress = ProgressDialog.show(.this, "Connecting...", "Please wait!!!");  //show a progress dialog
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try {
                if (btSocket == null || !isBtConnected) {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                }
            } catch (IOException e) {
                ConnectSuccess = false;//if the try failed, you can check the exception here
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess) {
                Toast.makeText(MainActivity.this, "Bluetooth Connection Failure", Toast.LENGTH_SHORT).show();
                //msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                //finish();
            } else {
                Toast.makeText(MainActivity.this, "Bluetooth Connected", Toast.LENGTH_SHORT).show();
                //msg("Connected.");
                isBtConnected = true;
                sendData("5t");
                updaterFunction();

            }
            //progress.dismiss();
        }


    }

    private void sendData(String message) {
        Log.d(TAG, "Sending data:" + message);
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write(message.getBytes());
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }

    private String receiveData() {
        try {
            TimeUnit.MILLISECONDS.sleep(600);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        byte[] buffer = new byte[1024];
        int bytes = 0;
        String string;
        if (btSocket != null) {
            try {
                bytes = btSocket.getInputStream().read(buffer);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        string = new String(buffer, 0, bytes);
        Log.d(TAG, "receiving data:"+ string);
        return string;
    }

}