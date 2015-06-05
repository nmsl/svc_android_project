package test.svc;

import java.io.File;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
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

public class StreamSelector extends Activity {
	private final String TAG = "Recorder";
		
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
	private String[] qualityType = {
			"Low",
			"Mid",
			"High"
	};
	
	private String output_folder	= "";
	private String output_file		= "";
	private String selectedVideo 	= "";
	private String wifiType 		= "WIFI"; 	// 3G or WIFI
	private int selectedQuality 	= 0;
	private int selectedMode		= 1;		// 0 = single HTTP request, 1 = DASH streaming
	
	// Spinner
	private Spinner  spin_video 	= null;
	private Spinner  spin_qulity 	= null;
	
	// Button
	private Button   btn_start 		= null;
	
	// TextView
	private TextView txt_runs		= null;
	private TextView txt_threads	= null;
	
	// EditText
	private EditText edit_output	= null;
	
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
	    this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.dash_layout);
		
		this.getViews ();
		this.setListener ();
		
		this.Init();
		
		btn_start.setEnabled(true);
	}
	
	private void Init() {
		SharedPreferences settings = getSharedPreferences("DASHSetting", MODE_PRIVATE);
		
		spin_video.setSelection(settings.getInt("videoIndex", 0));
		
		selectedQuality = settings.getInt("layerIndex", 0);
		spin_qulity.setSelection(selectedQuality);
		
		seekThreads.setProgress(settings.getInt("threadNo", 2)-1);
		seekRuns.setProgress(settings.getInt("runs", 2)-1);
				
		this.checkNetworkType();
		this.updateOutputName();
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
		//		spin_mode		= (Spinner)  this.findViewById (R.id.spin_mode);
		
		btn_start 		= (Button)   this.findViewById (R.id.btn_start);
		
		txt_runs		= (TextView) this.findViewById(R.id.lbl_runs);
		txt_threads		= (TextView) this.findViewById(R.id.lbl_threads);
				
		edit_output		= (EditText) this.findViewById(R.id.edit_output);
		
		networkType		= (RadioGroup) this.findViewById(R.id.group_network);
		btn_3g			= (RadioButton) this.findViewById(R.id.rbtn_3g);
		btn_wifi		= (RadioButton) this.findViewById(R.id.rbtn_wifi);
		
		seekRuns		= (SeekBar) this.findViewById(R.id.runs);
		seekThreads		= (SeekBar) this.findViewById(R.id.threads);
		
		this.setAdapterToSpinner(spin_video, videoNames);
		this.setAdapterToSpinner(spin_qulity, qualityExpr);
	}
	
	private void setListener () {
		// When video selected
		spin_video.setOnItemSelectedListener(new OnItemSelectedListener() {
			@Override
			public void onItemSelected(AdapterView<?> pView, View itemView, int position, long id) {
				selectedVideo = videoNames[position];
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
				updateOutputName();
			}
			@Override
			public void onNothingSelected(AdapterView<?> pView) {
				return;
			}
		});
		
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
			
		// When start button clicked
		btn_start.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View v) {				
				SharedPreferences.Editor editor = getSharedPreferences("DASHSetting", MODE_PRIVATE).edit();
				editor.putInt("videoIndex", spin_video.getSelectedItemPosition());
				editor.putInt("layerIndex", spin_qulity.getSelectedItemPosition());
				editor.putInt("threadNo", seekThreads.getProgress()+1);
				editor.putInt("runs", seekRuns.getProgress()+1);
				editor.commit();
				
				File folder = new File(output_folder);
				if (!folder.exists()) folder.mkdirs();
				
//				Log.i(TAG, "SelectedQulaity:" + selectedQuality);
//				Log.i(TAG, "VideoName:" + selectedVideo);
//				Log.i(TAG, "ThreadNo:" + (seekThreads.getProgress()+1));
//				Log.i(TAG, "output:" + output_file);
//				Log.i(TAG, "network:" + wifiType);
				
				Intent intent = new Intent(StreamSelector.this, Main.class);
				Bundle b = new Bundle();
				b.putInt("LayerIndex", selectedQuality);
				b.putInt("TemporalID", 3);
				b.putString("VideoName", selectedVideo);
				b.putInt("ThreadNo", (seekThreads.getProgress()+1));
				b.putString("output_file", output_file);
				intent.putExtras(b);
				startActivity(intent);
				
				// Toast.makeText(DASHSelector.this, "Waiting for playing....", Toast.LENGTH_LONG).show();
				
				// Close this Activity
				StreamSelector.this.finish();
			}
		});
	}
	
	private void updateOutputName () {
		output_folder = String.format("%s/%s/",
				Environment.getExternalStorageDirectory(),
				"svc/output/single");
		output_file = String.format("%s_%s_%s_%dT_R%d.txt",
				selectedVideo,
				qualityType[selectedQuality],
				wifiType,
				seekThreads.getProgress()+1,
				seekRuns.getProgress()+1);
		
		edit_output.setText(output_folder + output_file);
	}
	
	/**
	 * Set Data into Spinner
	 * 
	 * @param spin
	 *            The Spinner will be set the adapter
	 * @param array
	 *            The Data for Spinner
	 */
	private void setAdapterToSpinner(Spinner spin, String[] array) {
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_dropdown_item, array);
		spin.setAdapter(adapter);
	}
}
