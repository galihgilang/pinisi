package org.opencv.samples.colorblobdetect;

import java.util.ArrayList;
import java.util.List;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.imgproc.Imgproc;
import org.opencv.imgproc.Moments;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.ToggleButton;

public class ColorBlobDetectionActivity extends Activity implements OnTouchListener, CvCameraViewListener2 {
    private static final String  TAG              = "OCVSample::Activity";

    private boolean              mIsColorSelected = false;
    private Mat                  mRgba;
    private Scalar               mBlobColorRgba;
    private Scalar               mBlobColorHsv;
    private ColorBlobDetector    mDetector;
    private Mat                  mSpectrum;
    private Size                 SPECTRUM_SIZE;
    private Scalar               CONTOUR_COLOR;
    
    private	Button				buttonsend;
    		Button				buttonset;
    		EditText			text;
    		TextView			absis;
    		TextView			ordinat;
    		TextView			latitude;
    		TextView			longitude;
    		TextView			altitude;
    		TextView			range;
    		TextView			viewkompas;
    		ToggleButton		BluetoothToggle;
    		
    		SensorManager		sm;
    		Sensor				s;
    		
    		
    		BluetoothConnection	bc;
    		
    		double degree;

    private CameraBridgeViewBase mOpenCvCameraView;

    private BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    mOpenCvCameraView.enableView();
                    mOpenCvCameraView.setOnTouchListener(ColorBlobDetectionActivity.this);
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    public ColorBlobDetectionActivity() {
        Log.i(TAG, "Instantiated new " + this.getClass());
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.color_blob_detection_surface_view);

        bc = new BluetoothConnection();
        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.color_blob_detection_activity_surface_view);
        mOpenCvCameraView.setCvCameraViewListener(this);
        BluetoothToggle = (ToggleButton) findViewById(R.id.toggleButton1);
        BluetoothToggle.setChecked(bc.IsBluetoothOn());
        BluetoothToggle.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			
			@Override
			public void onCheckedChanged(CompoundButton arg0, boolean arg1) {
				// TODO Auto-generated method stub
				if(arg1) bc.BluetoothOn();
				else bc.BluetoothOff();
			}
		});
        text = (EditText) findViewById(R.id.editText1);
        absis = (TextView) findViewById(R.id.textView2);
        ordinat = (TextView) findViewById(R.id.textView1);
        altitude = (TextView) findViewById(R.id.textView3);
        latitude = (TextView) findViewById(R.id.textView4);
        longitude = (TextView) findViewById(R.id.textView5);
        range = (TextView) findViewById(R.id.textView6);
        viewkompas = (TextView) findViewById(R.id.textView7);
        
        
        buttonsend = (Button) findViewById(R.id.button2);
        buttonset = (Button) findViewById(R.id.button1);
        buttonsend.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if(bc.IsConnected()){
					bc.send(text.getText().toString());
				}
				else if(!bc.IsBluetoothOn())text.setText("Turn Bluetooth On!!"); 
				else {
					if(bc.setdevice()){
						bc.sambung();
						text.setText(bc.getdevicename());
					}
					else text.setText("device not found");
					if(bc.IsConnected())buttonsend.setText("Send");
				}
			}
		});
        buttonset.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if(bc.IsBluetoothOn()){
					bc.BluetoothOff();
				}
				else{
					bc.BluetoothOn();
				}
				
			}
		});
        
        callkompas();
        
        
    }
    
    void callkompas(){
    	sm = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        s = sm.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
        sm.registerListener(new SensorEventListener() {
			
			@Override
			public void onSensorChanged(SensorEvent event) {
				// TODO Auto-generated method stub
				double rad = Math.atan2(event.values[0], event.values[2]);
				degree = Math.toDegrees(rad) + 180;
				if(degree >=360)degree-=360;
				viewkompas.setText(String.valueOf(degree));
				
			}
			
			@Override
			public void onAccuracyChanged(Sensor sensor, int accuracy) {
				// TODO Auto-generated method stub
				//kompas.setText(String.valueOf(accuracy));
				
			}
		}, s, SensorManager.SENSOR_DELAY_UI);
        if (s != null){
          // Success! There's a magnetometer.
          }
        else {
          // Failure! No magnetometer.
          }
    }

    @Override
    public void onPause()
    {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this, mLoaderCallback);
    }

    public void onDestroy() {
        super.onDestroy();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    public void onCameraViewStarted(int width, int height) {
        mRgba = new Mat(height, width, CvType.CV_8UC4);
        mDetector = new ColorBlobDetector();
        mSpectrum = new Mat();
        mBlobColorRgba = new Scalar(255);
        mBlobColorHsv = new Scalar(255);
        SPECTRUM_SIZE = new Size(200, 16);
        CONTOUR_COLOR = new Scalar(255,0,0,255);
    }

    public void onCameraViewStopped() {
        mRgba.release();
    }

    public boolean onTouch(View v, MotionEvent event) {
        int cols = mRgba.cols();
        int rows = mRgba.rows();

        int xOffset = (mOpenCvCameraView.getWidth() - cols) / 2;
        int yOffset = (mOpenCvCameraView.getHeight() - rows) / 2;

        int x = (int)event.getX() - xOffset;
        int y = (int)event.getY() - yOffset;

        Log.i(TAG, "Touch image coordinates: (" + x + ", " + y + ")");

        if ((x < 0) || (y < 0) || (x > cols) || (y > rows)) return false;

        Rect touchedRect = new Rect();

        touchedRect.x = (x>4) ? x-4 : 0;
        touchedRect.y = (y>4) ? y-4 : 0;

        touchedRect.width = (x+4 < cols) ? x + 4 - touchedRect.x : cols - touchedRect.x;
        touchedRect.height = (y+4 < rows) ? y + 4 - touchedRect.y : rows - touchedRect.y;

        Mat touchedRegionRgba = mRgba.submat(touchedRect);

        Mat touchedRegionHsv = new Mat();
        Imgproc.cvtColor(touchedRegionRgba, touchedRegionHsv, Imgproc.COLOR_RGB2HSV_FULL);

        // Calculate average color of touched region
        mBlobColorHsv = Core.sumElems(touchedRegionHsv);
        int pointCount = touchedRect.width*touchedRect.height;
        for (int i = 0; i < mBlobColorHsv.val.length; i++)
            mBlobColorHsv.val[i] /= pointCount;

        mBlobColorRgba = converScalarHsv2Rgba(mBlobColorHsv);

        Log.i(TAG, "Touched rgba color: (" + mBlobColorRgba.val[0] + ", " + mBlobColorRgba.val[1] +
                ", " + mBlobColorRgba.val[2] + ", " + mBlobColorRgba.val[3] + ")");

        mDetector.setHsvColor(mBlobColorHsv);

        Imgproc.resize(mDetector.getSpectrum(), mSpectrum, SPECTRUM_SIZE);

        mIsColorSelected = true;

        touchedRegionRgba.release();
        touchedRegionHsv.release();

        return false; // don't need subsequent touch events
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
        mRgba = inputFrame.rgba();

        if (mIsColorSelected) {
            mDetector.process(mRgba);
            List<MatOfPoint> contours = mDetector.getContours();
            proseskontur(contours);
            Log.e(TAG, "Contours count: " + contours.size());
            Imgproc.drawContours(mRgba, contours, -1, CONTOUR_COLOR);

            Mat colorLabel = mRgba.submat(4, 20, 4, 20);
            colorLabel.setTo(mBlobColorRgba);

            Mat spectrumLabel = mRgba.submat(4, 4 + mSpectrum.rows(), 70, 70 + mSpectrum.cols());
            mSpectrum.copyTo(spectrumLabel);
        }

        //kompas.setText(String.valueOf(Kompas.degree));
        
        return mRgba;
    }
    
    public List<MatOfPoint> proseskontur(List<MatOfPoint> contours){
    	final int[] x = new int[contours.size()]; 
        final int[] y = new int[contours.size()];  
        double[] luas = new double[contours.size()]; 
        
        if(contours.size() >0){
        	for(int indeks = 0; indeks<contours.size();indeks++){
            	//cek luas per kontur
        		luas[indeks] = Imgproc.contourArea(contours.get(indeks));
            	//cek centroi per kontur
        		Moments momen = Imgproc.moments(contours.get(indeks));
            	double M00 = momen.get_m00();
            	double M01 = momen.get_m01();
            	double M10 = momen.get_m10();
            	x[indeks] = (int)(M10/M00); 
            	y[indeks] = (int)(M01/M00);
            };
        }
        
        //cari kontur terluas dan di tengah
        int indeksterluas = -1;
        final int tempi;
        double temp = 0;
        for (int indeks = 0; indeks<contours.size();indeks++){
        	if(luas[indeks]>temp/* && luas[indeks]>400 && y[indeks] > 200 && y[indeks] < 300*/){
        		temp = luas[indeks];
        		indeksterluas = indeks;
        	}
        }
        tempi = indeksterluas;
        
        //buat list of contour baru untuk ditampilkan
        List<MatOfPoint> tempcontours = new ArrayList<MatOfPoint>();
    	tempcontours.clear();
    	if(indeksterluas!= -1){
    		tempcontours.add(contours.get(indeksterluas));
    		runOnUiThread(new Runnable() {
                @Override
                public void run() {
                	absis.setText(String.valueOf(x[tempi]));
                	ordinat.setText(String.valueOf(y[tempi]));
                }
            });
    	}
    	else {
    		runOnUiThread(new Runnable() {
                @Override
                public void run() {
                	absis.setText(String.valueOf(tempi));
                	ordinat.setText(String.valueOf(tempi));
                }
            });
    		
    	}
    	return tempcontours;
    	
    }

    private Scalar converScalarHsv2Rgba(Scalar hsvColor) {
        Mat pointMatRgba = new Mat();
        Mat pointMatHsv = new Mat(1, 1, CvType.CV_8UC3, hsvColor);
        Imgproc.cvtColor(pointMatHsv, pointMatRgba, Imgproc.COLOR_HSV2RGB_FULL, 4);

        return new Scalar(pointMatRgba.get(0, 0));
    }
}