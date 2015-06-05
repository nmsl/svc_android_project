package my.app;

import java.io.File;

import test.svc.R;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.RadioGroup.OnCheckedChangeListener;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

public class DASHSelector extends Activity {
	private final String TAG = "DASH Selector";
	
	//---- Load Library ----//
	static {
		System.loadLibrary("dashFramework");
		System.loadLibrary("SDL2");
		System.loadLibrary("svc");
	}
	
	//---- Native Functions ----//
	private static native void 		nativeInitDASH();
	private static native boolean 	nativeParseMPD(String url);
	private static native void 		nativeOnSettingChanged(int quality_level);
	
	private String ServerIP = "140.114.79.68";
	private String MPDfile = "svc_splitall_dash.mpd";
	
	private void updateURL (int index) {
		selectedVideo = videoNames[index];
		selectedUrl = String.format("http://%s/Jargo/streaming/%s/dash/%s", ServerIP, selectedVideo, MPDfile);
	}
	
	private String[] videoNames = {
			"sport",
			"talking_head",
			"soap",
			"jeux",
			"doc-reality"
	};
	private String[] qualityExpr = {
			"Base Layer Only",
			"Base + Enh. 1",
			"Base + Enh. 1 + Enh. 2"
	};
	private String[] mode = {
			"Single Thread HTTP GET",
			"Multithreads DASH"
	};
	private String[] qualityType = {
			"Low",
			"Mid",
			"High"
	};
	
	private String output_folder	= "";
	private String output_file		= "";
	private String selectedVideo 	= "";
	private String selectedUrl 		= "";
	private String wifiType 		= "WIFI"; 	// 3G or WIFI
	private int selectedQuality 	= 0;
	private int selectedMode		= 1;		// 0 = single HTTP request, 1 = DASH streaming
	
	// Spinner
	private Spinner  spin_video 	= null;
	private Spinner  spin_qulity 	= null;
	//	private Spinner  spin_mode 		= null;
	
	// Button
	private Button   btn_download 	= null;
	private Button   btn_start 		= null;
	
	// TextView
	private TextView txt_video 		= null;
	private TextView txt_quality 	= null;
	private TextView txt_runs		= null;
	private TextView txt_threads	= null;
	private TextView txt_output		= null;
	
	// RadioGroup
	private RadioGroup networkType	= null;
	private RadioButton btn_3g		= null;
	private RadioButton btn_wifi	= null;
	
	// SeekBar
	private SeekBar seekRuns		= null;
	private SeekBar seekThreads		= null;

	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
//	    this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
	    this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.dash_layout);
		
		this.getViews ();
		this.setListener ();
		
		this.Init();
		
		btn_start.setEnabled(false);
	}
	
	private void Init() {
		SharedPreferences settings = getSharedPreferences("DASHSetting", MODE_PRIVATE);
		
		spin_video.setSelection(settings.getInt("videoIndex", 0));
		this.updateURL(spin_video.getSelectedItemPosition());
		
		selectedQuality = settings.getInt("layerIndex", 0);
		spin_qulity.setSelection(selectedQuality);
		
		seekThreads.setProgress(settings.getInt("threadNo", 2)-1);
		seekRuns.setProgress(settings.getInt("runs", 2)-1);
				
		this.checkNetworkType();
		this.updateOutputName();
		
		nativeInitDASH();
	}
	
	private void checkNetworkType() {
		ConnectivityManager conMan = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
		State mobile = conMan.getNetworkInfo(0).getState();
		State wifi = conMan.getNetworkInfo(1).getState();
		
		if (mobile == NetworkInfo.State.CONNECTED || mobile == NetworkInfo.State.CONNECTING) {
		    wifiType = "3G";
		    btn_3g.setChecked(true);
		} else if (wifi == NetworkInfo.State.CONNECTED || wifi == NetworkInfo.State.CONNECTING) {
			wifiType = "WIFI";
			btn_wifi.setChecked(true);
		}
	}
	
	private void getViews () {
		spin_video 		= (Spinner)  this.findViewById (R.id.spin_video);
		spin_qulity 	= (Spinner)  this.findViewById (R.id.spin_qulity);
		// spin_mode		= (Spinner)  this.findViewById (R.id.spin_mode);
		
		btn_download 	= (Button)   this.findViewById (R.id.btn_download);
		btn_start 		= (Button)   this.findViewById (R.id.btn_start);
		
		txt_video		= (TextView) this.findViewById (R.id.lbl_video);
		txt_quality		= (TextView) this.findViewById (R.id.lbl_quality);
		txt_runs		= (TextView) this.findViewById(R.id.lbl_runs);
		txt_threads		= (TextView) this.findViewById(R.id.lbl_threads);
		txt_output		= (TextView) this.findViewById(R.id.lbl_path);
		
		networkType		= (RadioGroup) this.findViewById(R.id.group_network);
		btn_3g			= (RadioButton) this.findViewById(R.id.rbtn_3g);
		btn_wifi		= (RadioButton) this.findViewById(R.id.rbtn_wifi);
		
		seekRuns		= (SeekBar) this.findViewById(R.id.runs);
		seekThreads		= (SeekBar) this.findViewById(R.id.threads);
		
		this.setAdapterToSpinner(spin_video, videoNames);
		this.setAdapterToSpinner(spin_qulity, qualityExpr);
		//		setAdapterToSpinner(spin_mode, mode);
		//		spin_mode.setSelection(1);
	}
	
	private void setListener () {
		// When video selected
		spin_video.setOnItemSelectedListener(new OnItemSelectedListener() {
			@Override
			public void onItemSelected(AdapterView<?> pView, View itemView, int position, long id) {
				updateURL(position);
				btn_start.setEnabled(false);
				updateOutputName();
			}
			@Override
			public void onNothingSelected(AdapterView<?> pView) {
				return;
			}
		});
		
		// When quality selected
		spin_qulity.setOnItemSelectedListener(new OnItemSelectedListener() {
			@Override
			public void onItemSelected(AdapterView<?> pView, View itemView, int position, long id) {
				selectedQuality = position;
				btn_start.setEnabled(false);
				updateOutputName();
			}
			@Override
			public void onNothingSelected(AdapterView<?> pView) {
				return;
			}
		});

		//		spin_mode.setOnItemSelectedListener(new OnItemSelectedListener() {
		//			@Override
		//			public void onItemSelected(AdapterView<?> pView, View itemView, int position, long id) {
		//				selectedMode = position;
		//				btn_start.setEnabled(false);
		//			}
		//			@Override
		//			public void onNothingSelected(AdapterView<?> pView) {
		//				return;
		//			}
		//		});
		
		// When NetworkType checked
		networkType.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged (RadioGroup group, int checkedID) {
				switch (checkedID) {
					case R.id.rbtn_3g:
						wifiType = "3G";
						break;
					case R.id.rbtn_wifi:
						wifiType = "WIFI";
						break;
					default:
						break;
				}
				
				updateOutputName();
			}
		});
		
		seekRuns.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			@Override
			public void onProgressChanged (SeekBar seekBar, int progress, boolean fromUser) {
				txt_runs.setText("Current Run:" + (progress+1));
				updateOutputName();
			}

			@Override
			public void onStartTrackingTouch (SeekBar seekBar) {				
			}

			@Override
			public void onStopTrackingTouch (SeekBar seekBar) {
			}
		});
		
		seekThreads.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			@Override
			public void onProgressChanged (SeekBar seekBar, int progress, boolean fromUser) {
				txt_threads.setText((progress+1) + " Thread(s)");
				updateOutputName();
			}

			@Override
			public void onStartTrackingTouch (SeekBar seekBar) {
			}

			@Override
			public void onStopTrackingTouch (SeekBar seekBar) {
			}
			
		});
		
		// When download button clicked
		btn_download.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {	
				SharedPreferences.Editor editor = getSharedPreferences("DASHSetting", MODE_PRIVATE).edit();
				editor.putInt("videoIndex", spin_video.getSelectedItemPosition());
				editor.putInt("layerIndex", spin_qulity.getSelectedItemPosition());
				editor.putInt("threadNo", seekThreads.getProgress()+1);
				editor.putInt("runs", seekRuns.getProgress()+1);
				editor.commit();
				
				new DownloadMPDAsyncTask(DASHSelector.this).execute(selectedUrl);
			}
		});
		
		
		// When start button clicked
		btn_start.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {
				nativeOnSettingChanged(selectedQuality+1);
				
				File folder = new File(output_folder);
				if (!folder.exists()) folder.mkdirs();
				
				Intent intent = new Intent(DASHSelector.this, SVCPlayer.class);
				Bundle b = new Bundle();
				b.putInt("LayerIndex", selectedQuality);
				b.putInt("TemporalID", 3);
				b.putString("VideoName", selectedVideo);
				b.putInt("StreamMode", selectedMode);
				b.putInt("ThreadNo", seekThreads.getProgress()+1);
				b.putString("output", txt_output.getText().toString());
				intent.putExtras(b);
				startActivity(intent);
				
				// Toast.makeText(DASHSelector.this, "Waiting for playing....", Toast.LENGTH_LONG).show();
				
				// Close this Activity
				DASHSelector.this.finish();
			}
		});
	}
	
	private void updateOutputName () {
		output_folder = String.format("%s/%s/",
				Environment.getExternalStorageDirectory().getAbsolutePath(),
				"svc/output/dash");
		output_file = String.format("%s_%s_%s_%dT_R%d.txt",
				selectedVideo,
				qualityType[selectedQuality],
				wifiType,
				seekThreads.getProgress()+1,
				seekRuns.getProgress()+1);
		
		txt_output.setText(output_folder + output_file);
	}
	
	/**
	 * Set Data into Spinner
	 * 
	 * @param spin
	 * 			The Spinner will be set the adapter
	 * @param array
	 * 			The Data for Spinner
	 */
	private void setAdapterToSpinner(Spinner spin, String[] array) {
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_dropdown_item, array);
		spin.setAdapter(adapter);
	}
	
	
	// Loading Icon
	class DownloadMPDAsyncTask extends AsyncTask<String, Void, Boolean> {
		public ProgressDialog mProgressDialog;
		private Activity mActivity;
		
		public DownloadMPDAsyncTask(Activity activity) {
			super();
			mActivity = activity;
		}
		
		@Override
		protected void onPreExecute() {
			super.onPreExecute();
			mProgressDialog = new ProgressDialog(mActivity);
			mProgressDialog.setMessage("Parsing MPD file ...");
			mProgressDialog.setCancelable(false);
			mProgressDialog.show();
		}

		@Override
		protected Boolean doInBackground(String... urls) {
			return nativeParseMPD(selectedUrl);
		}
		
		@Override
		protected void onPostExecute(Boolean result) {
			if (mProgressDialog != null && mProgressDialog.isShowing()) {
				mProgressDialog.dismiss();
			}
			
			if (result) {
				btn_start.setEnabled(true);
			}
			else {
				Toast.makeText(mActivity, "MPD file parse ERROR", Toast.LENGTH_LONG).show();
			}
			
			super.onPostExecute(result);
		}
	}
}
