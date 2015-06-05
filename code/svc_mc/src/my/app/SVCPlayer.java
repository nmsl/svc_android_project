package my.app;

import java.text.DecimalFormat;

import org.libsdl.app.SDLActivity;

import test.svc.R;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class SVCPlayer extends SDLActivity {
	private static final String TAG = "JavaMain";
	
	private int gopFrames = 16;
	DecimalFormat df_4 = new DecimalFormat("#0.0000");
	DecimalFormat df_2 = new DecimalFormat("#0.00");

	//---- Request URL is curBaseUrl/curPrefix/curPrefix_segmentNo_layerNo.264 ----//
	private String curBaseUrl 	= "http://140.114.79.68/streaming";
	private String curPrefix 	= "";
	
	String[] videos	= {
		/* 0 */ "doc-reality",
		/* 1 */ "jeux",
		/* 2 */ "soap",
		/* 3 */ "sport",
		/* 4 */ "talking_head" 
	};
	int[] layerIDs	= {
		/* 0 */ 16,
		/* 1 */ 32,
		/* 2 */ 48
	};
	
	private int curLayerID 		= layerIDs[0];
	private int curTemporalID 	= 3; // from 1 to 3
	private int curThreadNum 	= 1;
	private int curMode 		= 1;
	private String outputFile 	= "/sdcard/svc/output/test.txt"; 

	//---- Native function ----//
	// private static native int nativeDecodeSVC(Bitmap[] bitmapArray, int bitmapNum);
	private static native int 		nativeStartSVCDecoder(int mode);
	private static native void 		nativeSVCInit(int bitmapNum);
	private static native boolean	nativeIsDone();
	
	//-- Getter --//
	private static native int 		nativeGetWidth();
	private static native int 		nativeGetHeight();
	private static native int 		nativeGetLayerID();
	private static native int 		nativeGetTemporalID();
	//	private static native float 	nativeGetThroughput ();
	private static native float		nativeGetNetworkDelay();
	private static native float		nativeGetDecodeDelay();
	private static native int 		nativeGetSwitchGOP();
	private static native int 		nativeGetDownloadedGOPs();
	private static native int 		nativeGetDecodedGOPs();
	private static native float  	nativeGetCurrentFPS();
	private static native float  	nativeGetCumulativeFPS();
	private static native float		nativeGetThroughput();
	
	//-- Setter --//
	private static native void 		nativeSetFilePath(String baseUrl, String prefix);
	private static native void 		nativeSetOutputFilePath(String fileName);
	private static native void 		nativeSetLayerID(int layerID);
	private static native void 		nativeSetTemporalID(int temporalID);
	private static native void 		nativeSetThreadNum(int threadNum);
	private static native void 		nativeSetStartTime();
	
	//---- Views ----//
//	private TextView txt_videoName;
//	private TextView txt_threadNo;
//	private TextView txt_layerID;
//	private TextView txt_tmpID;
//	private TextView txt_frameInfo;
//	private TextView txt_switchNo;
//	private TextView txt_FPS;
//	private TextView txt_throughput;
//	private TextView txt_freeMEM;
	
	//---- Handler and Message Type ----//
	private Handler monitorHandler = null;
	private interface handlerType {
		int UPDATE_MONITOR = 0;
	}
	
	MemoryInfo mi = new MemoryInfo();
	ActivityManager activityManager;
	double availableMegs = 0.0f;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
//	    this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
	    this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		super.onCreate(savedInstanceState);
		Bundle b = this.getIntent().getExtras();
		activityManager = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
		
		long maxMemory = Runtime.getRuntime().maxMemory();
		Log.i(TAG, "MaxMemory : " + Long.toString(maxMemory/(1024*1024)) + " MB");
		
		/* Values Setting */
		if (b != null) {
			curTemporalID 	= b.getInt("TemporalID", 3);
			curThreadNum 	= b.getInt("ThreadNo", 1);
			curLayerID 		= layerIDs[b.getInt("LayerIndex", 0)];
			curPrefix 		= b.getString("VideoName", curPrefix);
			curMode 		= b.getInt("StreamMode", 1);
			outputFile 		= b.getString("output", "/sdcard/svc/output/test.txt");
		}
		
		// +-+-+-+-+-+-+-+-+-+-+-+- SVC Config +-+-+-+-+-+-+-+-+-+-+-+
		// setFilePath("/sdcard/sport.264");
		// setLayerID(32);
		// setTemporalID(32);
		// setThreadNum(3);
		// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		
		setViews();
		
		Init();
		nativeStartSVCDecoder(curMode);
		
		startThread();
	}
	
	private void startThread() {
		new Thread(new Runnable() {
			@Override
			public void run() {
				while (!nativeIsDone()) {
					try {
						monitorHandler.obtainMessage(handlerType.UPDATE_MONITOR).sendToTarget();
						Thread.sleep(1000);
					}
					catch (Exception e) {
						e.printStackTrace();
					}
				}
			}
		}).start();
	}
	
	private void setViews () {
		RelativeLayout.LayoutParams rlp = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		rlp.addRule(RelativeLayout.ALIGN_PARENT_TOP);
		rlp.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
		
		LayoutInflater layoutInflater = LayoutInflater.from(this);
		View monitor = layoutInflater.inflate(R.layout.monitor, null);
		monitor.setLayoutParams(rlp);
		 
//		txt_videoName 	= (TextView) monitor.findViewById(R.id.txt_videoName);
//		txt_threadNo 	= (TextView) monitor.findViewById(R.id.txt_threadNo);
//		txt_layerID 	= (TextView) monitor.findViewById(R.id.txt_layerID);
//		txt_tmpID 		= (TextView) monitor.findViewById(R.id.txt_tmpID);
//		txt_frameInfo	= (TextView) monitor.findViewById(R.id.txt_frameInfo);
//		txt_switchNo 	= (TextView) monitor.findViewById(R.id.txt_switchNo);
//		txt_FPS 		= (TextView) monitor.findViewById(R.id.txt_FPS);
//		txt_throughput 	= (TextView) monitor.findViewById(R.id.txt_throughput);
//		txt_freeMEM		= (TextView) monitor.findViewById(R.id.txt_freeMem);
//		
//		txt_videoName.setText(curPrefix);
//		txt_threadNo.setText("Threads:" + curThreadNum);
//		txt_layerID.setText("LayerID:" + curLayerID);
//		txt_tmpID.setText("TemporalID:" + curTemporalID);
//		txt_frameInfo.setText("0/0");
//		txt_switchNo.setText("Switch At:0");
//		txt_FPS.setText("FPS:(" + 0.0000 + "," + 0.0000 + ")");
//		txt_throughput.setText("Tp:" + 0 + " MBits");
//		
//		activityManager.getMemoryInfo(mi);
//		availableMegs = mi.availMem / 1048576f;
//		txt_freeMEM.setText("Free:" + df_2.format(availableMegs) + " MBytes");
//		
//		super.mLayout.addView(monitor);
	}
	
	private void Init () {
		/* Init Native */
		nativeSetStartTime();
		nativeSetThreadNum(curThreadNum); // need to call this before nativeSVCInit();
		nativeSVCInit(gopFrames);
		nativeSetLayerID(curLayerID);
		nativeSetTemporalID(curTemporalID);
		nativeSetFilePath(curBaseUrl, curPrefix); // It's used if streaming mode = 0
		nativeSetOutputFilePath(outputFile);

		monitorHandler = new Handler() {
			@Override
			public void handleMessage (Message msg) {
				switch (msg.what) {
					case handlerType.UPDATE_MONITOR: {
						activityManager.getMemoryInfo(mi);
						availableMegs = mi.availMem / 1048576f;
						
						int displayG = nativeGetDecodedGOPs();
						int downloadG = nativeGetDownloadedGOPs();
						int switchG = nativeGetSwitchGOP();
						
//						Log.i(TAG, "[c] - Cur GOP:" + displayG + "(" + switchG + ")");
						
//						txt_layerID.setText("LayerID:" + nativeGetLayerID());
//						txt_tmpID.setText("TemporalID:" + nativeGetTemporalID());
//						txt_frameInfo.setText(displayG + "/" + downloadG);
//						txt_switchNo.setText("Switch At:" + switchG);
//						txt_FPS.setText("FPS:(" + df_2.format(nativeGetCurrentFPS()) + "," + df_2.format(nativeGetCumulativeFPS()) + ")");
//						txt_throughput.setText("Tp:" + df_2.format(nativeGetThroughput()) + " Mbits");
//						txt_freeMEM.setText("Free:" + df_2.format(availableMegs) + " MBytes");
						// Log.i(TAG, "MEM:"+availableMegs);
						break;
					}
					default:
						break;
				}
				super.handleMessage(msg);
			}
		};
	}
}