package com.example.mybletestdemo;

import java.util.ArrayList;
import java.util.UUID;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattServerCallback;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.text.method.ScrollingMovementMethod;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

	private BluetoothManager myBluetoothManager;
	private BluetoothAdapter myBluetoothAdapter;
	private ArrayList<String> deviceAddressList;
	private BluetoothDevice myBluetoothDevice;
	private String myBluetoothDeviceAddress;
	private BluetoothGatt myBluetoothGatt;
	private BluetoothGattService myGattService;
	private BluetoothGattCharacteristic readGattCharacteristic;
	private BluetoothGattCharacteristic writeGattCharacteristic;

	private static final UUID UUID_SERVICE = UUID
			.fromString(SampleGattAttributes.MYCJ_BLE);
	public final static UUID UUID_HEART_RATE_MEASUREMENT = UUID
			.fromString(SampleGattAttributes.HEART_RATE_MEASUREMENT);
	private static final UUID uuid = UUID
			.fromString(SampleGattAttributes.MYCJ_BLE);
	private static final UUID UUID_READ = UUID
			.fromString(SampleGattAttributes.MYCJ_BLE_READ);
	private static final UUID UUID_WRITE = UUID
			.fromString(SampleGattAttributes.MYCJ_BLE_WRITE);

	private Handler myHandler;
	private Runnable myRunnable;

	private Button sendBtn2, sendBtn3, sendBtn4, sendBtn5;
	private EditText sendText2, sendText3, sendText4, sendText5;
	private ListView lv;
	private TextView textViewValues, tv_hint;
	private ProgressBar pbar;

	private StringBuffer sbValues;
	private byte[] data;
	private String strData;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		sendBtn2 = (Button) findViewById(R.id.sendBtn2);
		sendBtn3 = (Button) findViewById(R.id.sendBtn3);
		sendBtn4 = (Button) findViewById(R.id.sendBtn4);
		sendBtn5 = (Button) findViewById(R.id.sendBtn5);

		sendText2 = (EditText) findViewById(R.id.sendText2);
		sendText3 = (EditText) findViewById(R.id.sendText3);
		sendText4 = (EditText) findViewById(R.id.sendText4);
		sendText5 = (EditText) findViewById(R.id.sendText5);

		textViewValues = (TextView) findViewById(R.id.textViewValues);
		textViewValues.setMovementMethod(ScrollingMovementMethod.getInstance());

		sbValues = new StringBuffer();

		myHandler = new Handler();
		deviceAddressList = new ArrayList<String>();

		myBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
		myBluetoothAdapter = myBluetoothManager.getAdapter();

		if (myBluetoothAdapter.isEnabled()) {
			System.out.println("蓝牙已开启...");
		} else {
			Intent enableIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivity(enableIntent);
		}

	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);

	}

	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		((InputMethodManager) getSystemService(INPUT_METHOD_SERVICE))
				.hideSoftInputFromWindow(MainActivity.this.getCurrentFocus()
						.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
	}

	protected void onResume() {
		super.onResume();
		((InputMethodManager) getSystemService(INPUT_METHOD_SERVICE))
				.hideSoftInputFromWindow(MainActivity.this.getCurrentFocus()
						.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
	}

	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		super.onRestart();
	}

	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// TODO Auto-generated method stub
		System.out.println("-------------Call Menu onCreateOptionsMenu......");
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.menu_main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub

		switch (item.getItemId()) {
		case R.id.menu_main:
			Toast.makeText(getApplicationContext(), "查找蓝牙设备",
					Toast.LENGTH_SHORT).show();
			// scanLeDevice();
			reScanLeDevice(true);
			actionAlertDialog();
			break;

		default:
			break;
		}
		return super.onOptionsItemSelected(item);
	}

	public void sendBtn5(View view) {
		// 隐藏键盘
		((InputMethodManager) getSystemService(INPUT_METHOD_SERVICE))
				.hideSoftInputFromWindow(MainActivity.this.getCurrentFocus()
						.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);

		// String value = sendText5.getText().toString() + "\n";
		String value = sendText5.getText().toString().trim();
		writeStringToGatt(value);
	}

	public void sendBtn4(View view) {
		// 隐藏键盘
		((InputMethodManager) getSystemService(INPUT_METHOD_SERVICE))
				.hideSoftInputFromWindow(MainActivity.this.getCurrentFocus()
						.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);

		String value = sendText4.getText().toString().trim();
		writeStringToGatt(value);
	}

	public void sendBtn3(View view) {
		// 隐藏键盘
		((InputMethodManager) getSystemService(INPUT_METHOD_SERVICE))
				.hideSoftInputFromWindow(MainActivity.this.getCurrentFocus()
						.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);

		String value = sendText3.getText().toString().trim();
		writeStringToGatt(value);
	}

	public void sendBtn2(View view) {
		// 隐藏键盘
		((InputMethodManager) getSystemService(INPUT_METHOD_SERVICE))
				.hideSoftInputFromWindow(MainActivity.this.getCurrentFocus()
						.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);

		String value = sendText2.getText().toString().trim();
		writeStringToGatt(value);
	}

	public void scanLeDevice() {
		myHandler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				if (myBluetoothAdapter != null) {
					// myBluetoothAdapter.stopLeScan(myReLeScanCallback);
					myBluetoothAdapter.startLeScan(myReLeScanCallback);
				}
			}
		});

	}

	public void reScanLeDevice(final boolean enable) {
		if (deviceAddressList != null) {
			deviceAddressList.clear();
		}
		if (myBluetoothGatt != null) {
			myBluetoothGatt.close();
			myBluetoothGatt = null;
		}
		if (myBluetoothAdapter != null) {
			myBluetoothAdapter.stopLeScan(myReLeScanCallback);
			myBluetoothAdapter.startLeScan(myReLeScanCallback);
			myHandler.postDelayed(new Runnable() {

				@Override
				public void run() {
					// TODO Auto-generated method stub
					System.out.println("查找3秒后结束……");
					setView();
					myBluetoothAdapter.stopLeScan(myReLeScanCallback);
				}
			}, 3000);
			System.out.println("开始重新查找蓝牙设备");
		}
	}

	private BluetoothAdapter.LeScanCallback myReLeScanCallback = new BluetoothAdapter.LeScanCallback() {

		@Override
		public void onLeScan(BluetoothDevice device, int rssi, byte[] scanRecord) {
			// TODO Auto-generated method stub
			System.out.println("找到了 + Rssi : " + rssi + ""
					+ "  deviceAddress : " + device.getAddress());
			// if (myBluetoothGatt != null) {
			// myBluetoothGatt.close();
			// myBluetoothGatt = null;
			// }
			deviceAddressList.add(device.getAddress());
			System.out.println(deviceAddressList.size());
		}
	};

	public boolean connect(String paramString) {
		if ((myBluetoothAdapter == null) || (paramString == null))
			return false;
		if ((myBluetoothDeviceAddress != null)
				&& (paramString.equals(myBluetoothDeviceAddress))
				&& (myBluetoothGatt != null)) {
			if (myBluetoothGatt.connect()) {
				System.out.println("mBluetoothGatt不为空");
				return true;
			}
			return false;
		}
		BluetoothDevice localBluetoothDevice = myBluetoothAdapter
				.getRemoteDevice(paramString);
		if (localBluetoothDevice == null)
			return false;
		myBluetoothGatt = localBluetoothDevice.connectGatt(this, true,
				myGattCallback);
		myBluetoothDeviceAddress = paramString;
		return true;
	}

	private BluetoothGattCallback myGattCallback = new BluetoothGattCallback() {

		@Override
		public void onConnectionStateChange(BluetoothGatt gatt, int status,
				int newState) {
			// TODO Auto-generated method stub
			super.onConnectionStateChange(gatt, status, newState);

			if (newState == BluetoothProfile.STATE_CONNECTED) {
				System.err.println("连接上GATT！");
				myBluetoothGatt.getServices();
				myBluetoothGatt.discoverServices();
				sbValues.insert(0, "connected...\n");
				myHandler.post(new Runnable() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
						textViewValues.setText(sbValues);
					}
				});
			} else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
				System.out.println("连接断开！");
				sbValues.insert(0, "disconnected...\n");
				myHandler.post(new Runnable() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
						textViewValues.setText(sbValues);
					}
				});
			}
		}

		@Override
		public void onServicesDiscovered(BluetoothGatt gatt, int status) {
			// TODO Auto-generated method stub
			super.onServicesDiscovered(gatt, status);
			if (myBluetoothGatt != null) {
				myGattService = myBluetoothGatt.getService(uuid);
			}
			if (myGattService != null) {
				String aaa = myGattService.getUuid().toString();
				System.out.println("获得BLE GATT Services 成功 : " + aaa);
				readGattCharacteristic = myGattService
						.getCharacteristic(UUID_READ);

				writeGattCharacteristic = myGattService
						.getCharacteristic(UUID_WRITE);
			}
			if (readGattCharacteristic != null
					&& writeGattCharacteristic != null) {
				System.out.println("找到了READ和WRITE");

				// mBluetoothGatt.setCharacteristicNotification(
				// mGattCharacteristicWrite, true);
				setCharacteristicNotification(readGattCharacteristic, true);

				boolean test = myBluetoothGatt
						.readCharacteristic(readGattCharacteristic);
				System.out.println("READ状态 ： " + test);
			}

		}

		@Override
		public void onCharacteristicRead(BluetoothGatt gatt,
				BluetoothGattCharacteristic characteristic, int status) {
			// TODO Auto-generated method stub
			super.onCharacteristicRead(gatt, characteristic, status);
		}

		@Override
		public void onCharacteristicWrite(BluetoothGatt gatt,
				BluetoothGattCharacteristic characteristic, int status) {
			// TODO Auto-generated method stub
			super.onCharacteristicWrite(gatt, characteristic, status);
		}

		@Override
		public void onCharacteristicChanged(BluetoothGatt gatt,
				BluetoothGattCharacteristic characteristic) {
			// TODO Auto-generated method stub
			super.onCharacteristicChanged(gatt, characteristic);
			data = characteristic.getValue();
			if (data != null && data.length > 0) {
				strData = DataUtil.getStringByBytes(data) + "\n";
				System.out.println("收到的数据是 ： " + strData);

				sbValues.insert(0, strData);
				myHandler.post(new Runnable() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
						textViewValues.setText(sbValues);
					}
				});

			}
		}

		@Override
		public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
			// TODO Auto-generated method stub
			super.onReadRemoteRssi(gatt, rssi, status);
		}

	};

	public void disconnect() {
		if ((myBluetoothAdapter == null) || (myBluetoothGatt == null))
			return;
		myBluetoothGatt.disconnect();
		// mBluetoothGatt.close();
		// mBluetoothGatt = null;
	}

	public void setCharacteristicNotification(
			BluetoothGattCharacteristic characteristic, boolean enabled) {
		if (myBluetoothAdapter == null || myBluetoothGatt == null) {
			return;
		}
		myBluetoothGatt.setCharacteristicNotification(characteristic, enabled);

		// This is specific to Heart Rate Measurement.
		if (UUID_HEART_RATE_MEASUREMENT.equals(characteristic.getUuid())) {
			BluetoothGattDescriptor descriptor = characteristic
					.getDescriptor(UUID
							.fromString(SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG));
			descriptor
					.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
			myBluetoothGatt.writeDescriptor(descriptor);
		} else if (UUID_READ.equals(characteristic.getUuid())) {
			BluetoothGattDescriptor descriptor = characteristic
					.getDescriptor(UUID
							.fromString(SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG));
			descriptor
					.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
			myBluetoothGatt.writeDescriptor(descriptor);
		} else if (UUID_WRITE.equals(characteristic.getUuid())) {
			BluetoothGattDescriptor descriptor = characteristic
					.getDescriptor(UUID
							.fromString(SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG));
			descriptor
					.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
			myBluetoothGatt.writeDescriptor(descriptor);
		}
	}

	private void writeStringToGatt(String dataString) {
		if (dataString != null) {
			byte[] data = DataUtil.getBytesByString(dataString);
			writeGattCharacteristic.setValue(data);
			myBluetoothGatt.writeCharacteristic(writeGattCharacteristic);
		} else {
			return;
		}
	}

	private void getRssi() {
		myBluetoothGatt.readRemoteRssi();
	}

	private void actionAlertDialog() {
		AlertDialog.Builder builder;
		final AlertDialog alertDialog;
		View view = getLayoutInflater().inflate(R.layout.device_list, null);

		lv = (ListView) view.findViewById(R.id.device_list);
		tv_hint = (TextView) view.findViewById(R.id.tv);
		pbar = (ProgressBar) view.findViewById(R.id.pbar);
		// MyAdapter adapter = new MyAdapter(context, list);
		// myListView.setAdapter(adapter);
		// 创建对话框
		builder = new AlertDialog.Builder(MainActivity.this);
		builder.setView(view);
		builder.setPositiveButton("ReTry",
				new DialogInterface.OnClickListener() {
					// 重试按钮
					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub
						reScanLeDevice(true);
						actionAlertDialog();
						// lv.setVisibility(View.GONE);
						// tv_hint.setVisibility(View.GONE);
						// pbar.setVisibility(View.VISIBLE);
					}
				});
		builder.setNegativeButton("cancel",
				new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub
						// 取消查找设备的操作
						System.out.println("取消查找");
						dialog.dismiss();
					}
				});
		alertDialog = builder.create();
		alertDialog.show();

		lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				// TODO Auto-generated method stub
				TextView tv = (TextView) view;
				// Toast.makeText(MainActivity.this, "地址：" + tv.getText(),
				// Toast.LENGTH_SHORT).show();
				myBluetoothDeviceAddress = tv.getText().toString().trim();
				alertDialog.cancel();
				// 通知服务去连接
				connect(myBluetoothDeviceAddress);
			}
		});

	}

	public void setView() {
		if (deviceAddressList.size() > 0) {
			lv.setVisibility(View.VISIBLE);
			tv_hint.setVisibility(View.GONE);
			pbar.setVisibility(View.GONE);
			ArrayAdapter<String> adapter = new ArrayAdapter<>(
					MainActivity.this,
					android.R.layout.simple_list_item_single_choice,
					deviceAddressList);
			lv.setChoiceMode(ListView.CHOICE_MODE_SINGLE);
			lv.setAdapter(adapter);

		} else if (deviceAddressList.size() == 0) {
			lv.setVisibility(View.GONE);
			tv_hint.setVisibility(View.VISIBLE);
			tv_hint.setText("device not found!");
			pbar.setVisibility(View.GONE);
		}

	}
}
