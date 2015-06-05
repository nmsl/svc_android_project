package test.svc;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;
import android.widget.TextView;
import android.widget.Toast;

public class Main extends Activity {
	private static final String TAG = "JavaMain";
	
	Measurement myMeasur = null;

	UIhandler UIupdater = new UIhandler();
	SVChandler SVC_Handler;

	Bitmap[][] updateBitmap;
	int BitmapFilledCount[] = new int[2];
	int bitmapElementNumber = 16;
	int fps_sample_period = 5;
	int receivedGOP = 0;

	ShowView updateView;
	// TextView FPSmonitor;
	View FPSmonitor;

	int perm[][] = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 }, { 2, 0 }, { 2, 1 }, { 1, 2 }, { 2, 2 }, { 0, 2 } };
	int permID;

	/*
	 * Request URL is curBaseUrl/curPrefix/curPrefix_segmentNo_layerNo.264
	 */
	//	public String curFilePath 	= "/sdcard/svc/files/doc-reality.264";	// Not Used
	String curBaseUrl 	= "http://140.114.79.68/Jargo/streaming";
	String curPrefix 	= "sport";
	String output		= "/sdcard/svc/output/singl/tmp.txt";
	int layerIDs[] 		= {16, 32, 48};
	int curLayerID 		= 16; 	// from 0 to 32 (0~15, 16~31, 32~)
	int curTemporalID 	= 3; 	// from 1 to 3
	int curThreadNum 	= 1;	// 3 is best for our experiment
	int curRuns 		= 1;

	/*
	 *  for experiment
	 */
	MemoryInfo mem_info;
	android.os.Debug.MemoryInfo[] memInfoArray;
	ActivityManager am;

	DecimalFormat df_8 = new DecimalFormat("#0.00000000");
	DecimalFormat df_4 = new DecimalFormat("#0.0000");
	DecimalFormat df_2 = new DecimalFormat("#0.00");
	
	TextView txt_videoName;
	TextView txt_threadNo;
	TextView txt_layerID;
	TextView txt_tmpID;
	TextView txt_frameInfo;
	TextView txt_switchNo;
	TextView txt_FPS;
	TextView txt_throughput;

	// time measurement
	long start_time 			= 0;		// the timeStamp of start this app
	long firstPlay_time 		= 0;		// the timeStamp of first play the first frame
	long delay_time 			= 0;
	boolean checkStartupTime 	= false; 	// check weather already write startup time
	

	int samplingRate 		= 10;
	int maxSampleingCount	= 23;
	int samplingCounter 	= 0; 	// counter the number of sample in sampling_rate
	int GOPCounter 			= 0;
	

	float 	total_fps 		= 0f;
	long 	total_heapSize 	= 0;
	int 	total_pssSize	= 0;
	float 	total_TP 		= 0f;	// TP is short for ThroughPut
	
	int 	recordingCounter = 0;

	static {
		System.loadLibrary("svc"); 
	}
	
	class ThreadSignal {
		public static final int UPDATE_BUFFER_0 	= 0x0000;
		public static final int UPDATE_BUFFER_1 	= 0x0001;
		public static final int WRITE_BUFFER_0 		= 0x0010;
		public static final int WRITE_BUFFER_1 		= 0x0011;
		public static final int FINISH 				= 0x1111;
	}

	// Native functions
	private static native int 	nativeDecodeSVC(Bitmap[] bitmapArray, int bitmapNum);
	private static native int 	nativeGetWidth();
	private static native int 	nativeGetHeight();
	private static native int 	nativeGetLayerID();
	private static native int 	nativeGetTemporalID();
	private static native float nativeGetThroughput();
	private static native int 	nativeGetNetworkDelay();
	private static native long 	nativeGetDecodeDelay();
	private static native float nativeGetAvgDecodingTime();
	private static native int 	nativeGetSwitchGOP();
	private static native void 	nativeSetFilePath(String baseUrl, String prefix);
	private static native void 	nativeSetLayerID(int layerID);
	private static native void 	nativeSetTemporalID(int temporalID);
	private static native void 	nativeSetThreadNum(int threadNum);
	private static native void 	nativeSetStartTime();
	private static native void 	nativeClear();
	
	private void showArgs() {
		Log.i(TAG, "ThreadNo:" + curThreadNum);
		Log.i(TAG, "TemporalID:" + curTemporalID);
		Log.i(TAG, "LayerID:" + curLayerID);
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		start_time = System.currentTimeMillis();

		/* windows setting */
		requestWindowFeature( Window.FEATURE_NO_TITLE ); // remove title
		getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN ); // full screen

		/* Layout setting*/
		// LinearLayout MainLayout = new LinearLayout( this.getApplicationContext() );
		// MainLayout.setLayoutParams( new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT) );
		// MainLayout.setOrientation(LinearLayout.VERTICAL);
		RelativeLayout MainLayout = new RelativeLayout(this.getApplicationContext());
		MainLayout.setLayoutParams(new RelativeLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		this.setContentView(MainLayout);
		this.registerForContextMenu(MainLayout); // register Context Menu

		Init();
		MainLayout.addView(updateView);
		MainLayout.addView(FPSmonitor);

		/* Memory Measurement */
		am = (ActivityManager) this.getSystemService(ACTIVITY_SERVICE);
		mem_info = new ActivityManager.MemoryInfo();

		/* Init Native timer */
		nativeSetStartTime(); 

		Bundle b = this.getIntent().getExtras();
		if (b != null) {
			curTemporalID 	= b.getInt("TemporalID", 3);
			curThreadNum 	= b.getInt("ThreadNo", 1);
			curLayerID 		= layerIDs[b.getInt("LayerIndex", 0)];
			curPrefix 		= b.getString("VideoName", "sport");
			output 			= b.getString("output_file", "text.txt");
		}
		
		// showArgs();

		if(!Measurement.isInstanceExist()) {
			myMeasur = Measurement.getInstance(output);
		}
		myMeasur.openOutputFile();

		// +-+-+-+-+-+-+-+-+-+-+-+- SVC Config +-+-+-+-+-+-+-+-+-+-+-+
		// setFilePath("/sdcard/sport.264");
		// setLayerID(32);
		// setTemporalID(32);
		// setThreadNum(3);
		// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		
		new Thread() {
			@Override
			public void run() {
				Looper.prepare();
				SVC_Handler = new SVChandler();
				Looper.loop();
			}
		}.start();
			
		new Thread() {
			@Override
			public void run() {
				Looper.prepare();
//				SVC_Handler = new SVChandler();
				UIupdater.obtainMessage(ThreadSignal.FINISH).sendToTarget();
				Looper.loop();
			}
		}.start();
	}
	
	private void Init() {
		updateBitmap = new Bitmap[2][];
		updateBitmap[0] = new Bitmap[bitmapElementNumber];
		updateBitmap[1] = new Bitmap[bitmapElementNumber];

		AllocBitmap(1, 1); // allocation two buffers of bitmapElementNumber of one by one bitmaps.

		// set surface View
		updateView = new ShowView(this.getApplicationContext());
		RelativeLayout.LayoutParams lp_updateView = new RelativeLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		// lp_updateView.topMargin = 51;
		updateView.setLayoutParams(lp_updateView);
		updateView.Init(updateBitmap, BitmapFilledCount);

		RelativeLayout.LayoutParams rlp = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		rlp.addRule(RelativeLayout.ALIGN_PARENT_TOP);
		rlp.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
		
		LayoutInflater layoutInflater = LayoutInflater.from(this);
		FPSmonitor = layoutInflater.inflate(R.layout.monitor, null);
		FPSmonitor.setLayoutParams(rlp);
		 
		txt_videoName 	= (TextView) FPSmonitor.findViewById(R.id.txt_videoName);
		txt_threadNo 	= (TextView) FPSmonitor.findViewById(R.id.txt_threadNo);
		txt_layerID 	= (TextView) FPSmonitor.findViewById(R.id.txt_layerID);
		txt_tmpID 		= (TextView) FPSmonitor.findViewById(R.id.txt_tmpID);
		txt_frameInfo	= (TextView) FPSmonitor.findViewById(R.id.txt_frameInfo);
		txt_FPS 		= (TextView) FPSmonitor.findViewById(R.id.txt_FPS);
		txt_throughput 	= (TextView) FPSmonitor.findViewById(R.id.txt_throughput);
		
		txt_videoName.setText(curPrefix);
		txt_threadNo.setText("Threads:" + curThreadNum);
		txt_layerID.setText("LayerID:" + curLayerID);
		txt_tmpID.setText("TemporalID:" + curTemporalID);
		txt_frameInfo.setText("0 (0)");
		txt_FPS.setText("FPS:(" + 0.00 + "," + 0.00 + ")");
		txt_throughput.setText("Tp:" + 0 + " MBits");
		
//		FPSmonitor = new TextView(this.getApplicationContext());
//		FPSmonitor.setTextSize(18);
//		FPSmonitor.setBackgroundColor(Color.BLUE);
//		RelativeLayout.LayoutParams lp_FPSmonitor = new RelativeLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
//		// lp_FPSmonitor.topMargin = 400;
//		FPSmonitor.setLayoutParams(lp_FPSmonitor);
	}

	protected static final int INC_L = Menu.FIRST;
	protected static final int DEC_L = Menu.FIRST+1;
	protected static final int INC_T = Menu.FIRST+2;
	protected static final int DEC_T = Menu.FIRST+3;

	@Override 
	public boolean onCreateOptionsMenu(Menu menu) {
		menu.add(0, INC_L, 0, "+ Spatial");
		menu.add(0, DEC_L, 0, "- Spatial");
		menu.add(0, INC_T, 0, "+ Temporal");
		menu.add(0, DEC_T, 0, "- Temporal");
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {

			case INC_L :
				if (curLayerID == 16) {
					curLayerID = 32;
					nativeSetLayerID(curLayerID);
				}
				else if (curLayerID == 32) {
					curLayerID = 48;
					nativeSetLayerID(curLayerID);
				}
				break;
			case DEC_L :
				if (curLayerID == 48) {
					curLayerID = 32;
					nativeSetLayerID(curLayerID);
				}
				else if (curLayerID == 32) {
					curLayerID = 16;
					nativeSetLayerID(curLayerID);
				}
				break;
			case INC_T :
				if (curTemporalID < 3) {
					curTemporalID++;
					nativeSetTemporalID(curTemporalID);
				}
				break;
			case DEC_T :
				if (curTemporalID > 1) {
					curTemporalID--;
					nativeSetTemporalID(curTemporalID);
				}
				break;
		}

		Toast.makeText(getApplicationContext(), "Current SID = " + curLayerID + ", TID = " + curTemporalID, Toast.LENGTH_LONG).show();

		return super.onOptionsItemSelected(item);
	}

	private void AllocBitmap(int W, int H) {
		for (int i = 0; i < bitmapElementNumber; ++i) {
			if (updateBitmap[0][i] != null)
				updateBitmap[0][i].recycle();

			if (updateBitmap[1][i] != null)
				updateBitmap[1][i].recycle();

			Bitmap source = Bitmap.createBitmap(W, H, Bitmap.Config.RGB_565);
			updateBitmap[0][i] = source;
			updateBitmap[1][i] = source;
		}
	}

	/* UI Handler */
	class UIhandler extends Handler {
		public int curProgress;
		private boolean startCounter;
		private int displayFrames, totalFrames;
		private double totalFrameRate;
		private long time_start, time_end, time_orig;

		public void Init() {
			totalFrames 			= 0;
			displayFrames 			= 0;
			time_start = time_orig 	= System.currentTimeMillis();
			startCounter 			= false;
		}

		public UIhandler() {
			Init();
		} 

		@Override
		public void handleMessage(Message msg) {

			switch (msg.what) {
				case ThreadSignal.UPDATE_BUFFER_0:
					SVC_Handler.obtainMessage(ThreadSignal.WRITE_BUFFER_1).sendToTarget();
					updateView.Update(0);
					displayFrames = BitmapFilledCount[0];
					break;

				case ThreadSignal.UPDATE_BUFFER_1:
					SVC_Handler.obtainMessage(ThreadSignal.WRITE_BUFFER_0).sendToTarget();
					updateView.Update(1);
					displayFrames = BitmapFilledCount[1];
					break;

				case ThreadSignal.FINISH:
					this.removeMessages(ThreadSignal.UPDATE_BUFFER_1);
					this.removeMessages(ThreadSignal.UPDATE_BUFFER_0);
					SVC_Handler.removeMessages(ThreadSignal.WRITE_BUFFER_0);
					SVC_Handler.removeMessages(ThreadSignal.WRITE_BUFFER_1);

					//+-+-+-+-+-+-+- Set value to SVC decoder +-+-+-+-+-+-+
					nativeSetThreadNum(curThreadNum);
					nativeSetLayerID(curLayerID);
					nativeSetTemporalID(curTemporalID);
					nativeSetFilePath(curBaseUrl, curPrefix);
					//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
					Init();
					SVC_Handler.Init();
					SVC_Handler.obtainMessage(ThreadSignal.WRITE_BUFFER_0).sendToTarget();
					break;
			}

			if (displayFrames > 0) {
				time_end = System.currentTimeMillis();
				totalFrames += displayFrames;
				double frameRate = ((double) displayFrames * 1000f) / (time_end - time_start);
				totalFrameRate = ((double) totalFrames * 1000f) / (time_end - time_orig);

				time_start = System.currentTimeMillis();

				float tp = nativeGetThroughput();
				// Log.i(TAG, "Throuput = " + tp);
				txt_threadNo.setText("Threads:" + curThreadNum);
				txt_layerID.setText("LayerID:" + nativeGetLayerID());
				txt_tmpID.setText("TemporalID:" + nativeGetTemporalID());
				txt_frameInfo.setText("GOPs:" + totalFrames/16 + " (" + nativeGetSwitchGOP() + ")");
				txt_FPS.setText("FPS:(" + df_2.format(frameRate) + "," + df_2.format(totalFrameRate) + ")");
				txt_throughput.setText("Tp:" + df_2.format(tp) + " Mbits");
				
				// FPSmonitor.setText(
				// "Running ID : " + String.valueOf(curProgress) +
				// ", T: " + nativeGetTemporalID() + ", L: " + nativeGetLayerID() +
				// ", Thread : " + String.valueOf(curThreadNum) +
				// " ---> " + curPrefix + "(" + total_counter + " : " + nativeGetSwitchGOP() + ")\n" +
				// "Current : " + df_8.format(frameRate) +
				// ", Cumulative : " + df_8.format(totalFrameRate));
				// FPSmonitor.invalidate();

				// am.getMemoryInfo(mem_info);
				// memInfoArray = am.getProcessMemoryInfo(pid);

				GOPCounter++;
				if (frameRate > 1) {
					total_fps += frameRate;
					// total_heapSize += Debug.getNativeHeapAllocatedSize();
					// total_pssSize += memInfoArray[0].getTotalPrivateDirty();
					total_TP += tp;
					samplingCounter ++;
				}

				// if (GOPCounter % sample_rate_of_gop == 0 || (GOPCounter == 563)) {
				if ((GOPCounter % samplingRate == 0) && (recordingCounter < maxSampleingCount)) {
					doExperiment(displayFrames);
					Log.v(TAG, "TESTSEG : wrote log into file, at GOPs: " + GOPCounter);

					if (recordingCounter >= maxSampleingCount) {
						Log.e(TAG, "****** EXIT SYSTEM ******");
//						myMeasur.writeLineIntoFile(String.format("(%d) %d=>AVGRT:%f\n", recordingCounter-1, System.currentTimeMillis(), updateView.getAVGRenderingTime()));
						myMeasur.closeOutputFile();
						System.exit(0);
					}
				}

				// skip first 50 frames
				if (!startCounter && totalFrames > 50) {
					startCounter = true;
					totalFrames = 0;
					time_orig = System.currentTimeMillis();
				}
			}

			super.handleMessage(msg);
		}
	}
	/* End UIhandler */

	// private int pid[] = {android.os.Process.myPid()};

	// Record all values of experiment.
	// including TransferDelay, DecodeDelay, FPS, and ThroughPut
	private void doExperiment(int frames) {
		String startup = "";
		if (firstPlay_time > 0) {
			if (!checkStartupTime) {
				Log.i(TAG, "Decoding Time = " + nativeGetDecodeDelay());
				startup = String.format("(%d) %d=>TrD:%d,DeD:%d\n", recordingCounter, System.currentTimeMillis(), nativeGetNetworkDelay(), nativeGetDecodeDelay()*16/frames);
				// startup = System.currentTimeMillis() + "=>TrD:" + nativeGetNetworkDelay() + ",DeD:" + nativeGetDecodeDelay() + "\n";
				checkStartupTime = true;
			}
		}

		String output_line = startup +
				String.format("(%d) %d=>TP:%s,FPS:%s,AVGRT:%.2f\n",
						recordingCounter++,
						System.currentTimeMillis(),
						df_2.format(total_TP/samplingCounter),
						df_2.format(total_fps/samplingCounter),
						updateView.getAVGRenderingTime()*16);
				// System.currentTimeMillis() + "=> " + "FPS:" + df_2.format(total_fps / sampling_counter) + ","
				// + "Mem Usage:" + df_4.format(((((float)total_heapSize/sampling_counter)/1024.0) + ((float)total_pssSize/sampling_counter))/1024.0) + ","
				// + "CPU usage:" + df_8.format(readUsage(android.os.Process.myPid())) + ","
				// + "TP:" + df_2.format(total_TP / sampling_counter) + ",\n";

		myMeasur.writeLineIntoFile(output_line);

		// clear all data
		delay_time = System.currentTimeMillis();
		total_fps = 0f;
		// total_heapSize = 0;
		// total_pssSize = 0;
		total_TP = 0;
		samplingCounter = 0;
	}

	/*
	 * calculate total CPU utilization for this APP
	 * but the value has some problem, is always less than 25%
	 * so, we do this experiment later.
	 */
	// boolean first = true;
	// long cpu1, cpu2, idle1, idle2;
	// private float readUsage(int pid) {
	// String load = "";
	// String[] toks;
	//
	// try {
	// RandomAccessFile reader = new RandomAccessFile("/proc/" + pid + "/stat", "r");
	// /*
	// * toks value detail:
	// * http://blog.csdn.net/pppjob/article/details/4060336
	// */
	//
	// // if (first) {
	// // load = reader.readLine();
	// // toks = load.split("\\s+");
	// // idle1 = Long.parseLong(toks[4]);
	// // cpu1 = Long.parseLong(toks[1]) + Long.parseLong(toks[2]) + Long.parseLong(toks[3])
	// // + Long.parseLong(toks[5]) + Long.parseLong(toks[6]) + Long.parseLong(toks[7]);
	// // cpu1 = Long.parseLong(toks[11]) + Long.parseLong(toks[12]);
	// // first = false;
	// // }
	// // else {
	// // idle1 = idle2;
	// // cpu1 = cpu2;
	// // }
	//
	// // reader.seek(0);
	// load = reader.readLine();
	// reader.close();
	//
	// toks = load.split("\\s+");
	//
	// // idle2 = Long.parseLong(toks[4]);
	// // cpu2 = Long.parseLong(toks[1]) + Long.parseLong(toks[2]) + Long.parseLong(toks[3])
	// // + Long.parseLong(toks[5]) + Long.parseLong(toks[6]) + Long.parseLong(toks[7]);
	// cpu2 = Long.parseLong(toks[13]) + Long.parseLong(toks[14]);
	// Log.d(TAG, "toks[11]: " + toks[13] + ", toks[12]: " + toks[14]);
	//
	// // if ((cpu2 - cpu1 == 0) && (idle2 - idle1 == 0)) {
	// // if(cpu2 == cpu1) {
	// // return 0;
	// // }
	//
	// // float result = ((float)(cpu2-cpu1) / ((cpu2 + idle2) - (cpu1 + idle1)));
	// Log.d(TAG, "time perio: " + (System.currentTimeMillis() - start_time));
	// double result = ((cpu2) / (((System.currentTimeMillis() - start_time) * 996.14 / 1000)));
	// Log.d(TAG, "result: " + result);
	//
	// return (float) result;
	// } catch (IOException ex) {
	// ex.printStackTrace();
	// }
	//
	// return -1;
	// }

	/* SVC handler */
	class SVChandler extends Handler {
		private boolean alerted;

		public void Init() {
			alerted = false;
		}

		public SVChandler() {
			Init();
		}

		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
				case ThreadSignal.WRITE_BUFFER_0:
					BitmapFilledCount[0] = nativeDecodeSVC(updateBitmap[0], bitmapElementNumber);
					if (firstPlay_time == 0) firstPlay_time = System.currentTimeMillis();

					if (BitmapFilledCount[0] >= 0) {
						Log.i(TAG, "Received GOPNo = " + receivedGOP++);
						UIupdater.obtainMessage(ThreadSignal.UPDATE_BUFFER_0).sendToTarget();
					}
					else if (BitmapFilledCount[0] == -1) {
						int W = nativeGetWidth();
						int H = nativeGetHeight();
						AllocBitmap(W, H);
						SVC_Handler.obtainMessage(ThreadSignal.WRITE_BUFFER_0).sendToTarget();
					}
					else if (BitmapFilledCount[0] == -2) {
						// Do Nothing
					}

					// Log.d(TAG, "BitmapFilledCount[0] = " + BitmapFilledCount[0]);
					break;

				case ThreadSignal.WRITE_BUFFER_1:
					BitmapFilledCount[1] = nativeDecodeSVC(updateBitmap[1], bitmapElementNumber);
					if (firstPlay_time == 0) firstPlay_time = System.currentTimeMillis();

					if (BitmapFilledCount[1] >= 0) {
						Log.i(TAG, "Received GOPNo = " + receivedGOP++);
						UIupdater.obtainMessage(ThreadSignal.UPDATE_BUFFER_1).sendToTarget();
					}
					else if (BitmapFilledCount[1] == -1) {
						int W = nativeGetWidth();
						int H = nativeGetHeight();
						AllocBitmap(W, H);
						SVC_Handler.obtainMessage(ThreadSignal.WRITE_BUFFER_1).sendToTarget();
					}
					else if (BitmapFilledCount[1] == -2) {
						// Do Nothing
					}

					// Log.d(TAG, "BitmapFilledCount[1] = " + BitmapFilledCount[1]);
					break;
			}
			super.handleMessage(msg);
		}
	} // End SVC handler

	public void appendLog(String text) {
		File logFile = new File("sdcard/svc/svc_log.txt");
		if (!logFile.exists()) {
			try {
				logFile.createNewFile();
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		}

		try {
			BufferedWriter buf = new BufferedWriter(new FileWriter(logFile, true));
			buf.append(text);
			buf.newLine();
			buf.close();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}
}


/* Customized SurfaceView called ShowView */
class ShowView extends SurfaceView {
	private static final String TAG = "ShowView";
	private static final boolean FULL_SCREEN = false;
	
	private Bitmap updateBitmap[][];
	private int BitmapFilledCount[];
	private Canvas canvas;
	private SurfaceHolder viewHolder;
	private int displayGOP = 0;
	
	private float renderingTotalTime = 0;
	private int renderingCounter = 0;

	public ShowView(Context context) { 
		super(context);
		viewHolder = this.getHolder(); 
	}

	public void Init(Bitmap[][] bitmapSrc, int BitmapFilledCountSrc[]) {
		updateBitmap = bitmapSrc; 
		BitmapFilledCount = BitmapFilledCountSrc;
	}
	
	public float getAVGRenderingTime () {
		return renderingTotalTime/(float)(renderingCounter);
	}

	public void Update(int buffer_index) {
		// int W = this.getWidth();
		// int H = this.getHeight();
		Log.i(TAG, "Display GOP = " + displayGOP++);
		
		long timestart = System.currentTimeMillis();
		for (int i = 0; i < BitmapFilledCount[buffer_index]; ++i) {			
			canvas = viewHolder.lockCanvas();
			canvas.drawColor(Color.BLACK); // clean background
			canvas.drawBitmap(updateBitmap[buffer_index][i], 0, 0, null);
			
			// set as a full-screen image, it's will be slower
			// if (FULL_SCREEN) {
			// Bitmap resizeBmp = Bitmap.createScaledBitmap(updateBitmap[buffer_index][i], W, H, false);
			// canvas.drawBitmap(resizeBmp, 0, 0, null);
			// }
			viewHolder.unlockCanvasAndPost(canvas);

			/*--- set delay of display frames from GOP ---*/
			// try {
			// Thread.sleep(33); // default 33
			// } catch (InterruptedException e) {
			// e.printStackTrace();
			// }
		} // End for loop
		
		renderingTotalTime += (System.currentTimeMillis()-timestart);
		renderingCounter += BitmapFilledCount[buffer_index];
		
		Log.i(TAG, "Display GOP Done");
	} // End Update
}
/* End ShowView */
