package test.svc;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import android.os.Environment;
import android.util.Log;

public class Measurement {
	private static final boolean WRITE = true;
	
	private static Measurement mea = null;

	private BufferedWriter bw = null;
	
	private String prefix 		= "talking_head";
	private String quality 		= "Low";
	private String networkType 	= "WIFI";
	private String outputPath 	= "/sdcard/svc/output/single";
	private String fileName 	= "test.txt";
	private int threads			= 1;
	private int times 			= 0;
	
	static public Measurement getInstance(String prefix, int layerID, String networkType, int threads, int runs) {
		if (mea == null) 
			mea = new Measurement(prefix, layerID, networkType, threads, runs);
		
		return mea;
	}
	
	static public Measurement getInstance(String absPath) {
		if (mea == null)
			mea = new Measurement(absPath);
		return mea;
	}
	
	static public void releaseInstance() {
		mea = null;
	}
	
	static public boolean isInstanceExist() {
		if (mea == null) return false;
		
		return true;
	}
	
	public Measurement(String output_path) {
		fileName = output_path;
	}

	public Measurement(String prefix, int layerID, String type, int threads, int runs) {
		setPrefix(prefix);
		setLayerID(layerID);
		set3G(type);
		setThreads(threads);
		setRuns(runs);
	}
	
	public void openOutputFile() {
		if (!WRITE) return;
		
		outputPath = String.format("%s/%s", Environment.getExternalStorageDirectory(), "svc/output/single");
		
		try {
			File f = new File(outputPath);
			if (!f.exists())
				f.mkdirs();

			FileWriter fw = new FileWriter(f.getAbsolutePath() + "/" + fileName, true);
			bw = new BufferedWriter(fw);
		}
		catch (IOException ioe) {
			Log.e("Measurement", "File open IOException: " + ioe);
		}
	}
	
	public void writeLineIntoFile(String line) {
		if (!WRITE) return;
		
		if (bw == null) {
			openOutputFile();
		}		
		
		try {
			bw.write(line);
			bw.flush();
		}
		catch (IOException e) {
			Log.e("Measurement", "File wirte IOException: " + e);
		}
	}
	
	public void closeOutputFile() {
		if (bw != null) {
			try {
				bw.flush();
				bw.close();
			}
			catch (IOException e) {
				Log.e("Measurement", "File close IOException: " + e);
			}
		}
		
		bw = null;
		releaseInstance();
	}
	
	private void updateFilaName() {
		fileName = String.format("%s_%s_%s_%dT_R%d.txt",
				this.prefix, this.quality, this.networkType, this.threads, this.times);
	}
	
	private void setThreads(int threadNo) {
		this.threads = threadNo;
		updateFilaName();
	}
	
	private void setRuns(int run) {
		times = run;
		updateFilaName();
	}
	
	private void setPrefix(String pre) {
		prefix = pre;
		updateFilaName();
	}
	
	private void setLayerID(int layerid) {
		switch (layerid) {
			case 16:
				quality = "Low";
				break;
			case 32:
				quality = "Mid";
				break;
			case 33:
				quality = "High";
				break;
			default:
				quality = "Low";
				break;
		}
		updateFilaName();
	}
	
	private void set3G(String type) {
		networkType = type;
		updateFilaName();
	}
}
