package shaheen.taha.bluetoothhomev2;

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

import java.io.DataInputStream;
import java.io.IOException;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button bulbButton = (Button) findViewById(R.id.bulbButton);
        final Button fanButton = (Button) findViewById(R.id.fanButton);
        final Button switchBoardButton = (Button) findViewById(R.id.switchBoardButton);
        final Button plugButton = (Button) findViewById(R.id.plugButton);

        bulbButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendData("1t");
                bulbButton.setBackgroundResource(R.color.ba000d);
            }
        });

        fanButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendData("2t");
                fanButton.setBackgroundResource(R.color.b0003a);
            }
        });

        switchBoardButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendData("3t");
                switchBoardButton.setBackgroundResource(R.color.ba000d);
            }
        });

        plugButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendData("4t");
                plugButton.setBackgroundResource(R.color.b0003a);
            }
        });


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
    private static String address = "98:D3:31:FB:61:26";
    private static String TAG="fromPhone";


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
                if (btSocket == null || !isBtConnected)
                {
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
                sendData("rt");
                receiveData();
            }
            //progress.dismiss();
        }


    }
    private void sendData(String message) {
        Log.d(TAG, "...Sending data: " + message + "...");
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write(message.getBytes());
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }
        }

    }
    private void receiveData() {
        if (btSocket!=null)
        {
            try
            {
                DataInputStream dinput = new DataInputStream(btSocket.getInputStream());
                byte[] byteArray = new byte[0];
                dinput.readFully(byteArray, 0, byteArray.length);
                Log.d(TAG, "...Receiving data: " + byteArray + "...");
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }
        }

    }


}
