package my.app;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import android.util.Log;

public class Measurement {
	private static final boolean WRITE = false;
	
	private static Measurement mea = null;

	private BufferedWriter bw = null;
	
	private String prefix = "talking_head";
	private String size = "L";
	private String with3G = "wifi";
	private String outputPath = "/sdcard/svc/output/";
	private String fileType = ".txt";
	private String fileName = "default" + fileType;
	private int times = 0;
	private int gopSize = 25;
	
	static public Measurement getInstance(boolean is3G, int layerID, String prefix, int loops, int gop_frames) {
		if (mea == null) 
			mea = new Measurement(is3G, layerID, prefix, loops, gop_frames);
		
		return mea;
	}
	
	static public void releaseInstance() {
		mea = null;
	}
	
	static public boolean isInstanceExist() {
		if (mea == null) return false;
		
		return true;
	}
	
	public Measurement(boolean is3G, int layerID, String prefix, int loops, int gop_frames) {
		setGOPsize(gop_frames);
		setRuns(loops);
		setPrefix(prefix);
		setLayerID(layerID);
		set3G(is3G);
	}
	
	public void openOutputFile() {
		if (!WRITE) return;
		
		try {
			File f = new File(outputPath);
			if (!f.exists())
				f.mkdirs();

			FileWriter fw = new FileWriter(f.getAbsolutePath() + "/" + fileName + fileType);
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
		fileName = String.format("%s_%s_%d_S%d_%s", this.prefix, this.size, this.times, this.gopSize, this.with3G);
	}
	
	
	private void setGOPsize(int framesPerGOP) {
		gopSize = framesPerGOP;
		updateFilaName();
	}
	
	private void setRuns(int loop) {
		times = loop;
		updateFilaName();
	}
	
	private void setPrefix (String pre) {
		prefix = pre;
		updateFilaName();
	}
	
	private void setLayerID (int layerid) {
		switch (layerid) {
		case 16:
			size = "L";
			break;
		case 32:
			size = "M";
			break;
		case 33:
			size = "H";
			break;
		default:
			size = "L";
			break;
		}
		updateFilaName();
	}
	
	private void set3G (boolean is3G) {
		if(is3G) with3G = "3G";
		else with3G = "wifi";
		updateFilaName();
	}
	
}
