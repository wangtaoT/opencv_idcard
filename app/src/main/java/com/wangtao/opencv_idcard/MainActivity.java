package com.wangtao.opencv_idcard;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.Image;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private Button btn_rt;
    private ImageView img_1;

    static {
        System.loadLibrary("OpenCV");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btn_rt = findViewById(R.id.btn_rt);
        img_1 = findViewById(R.id.img_1);

        btn_rt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                rt(v);
            }
        });
    }

    public void rt(View view) {
        Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.id_card1);
        Bitmap template = BitmapFactory.decodeResource(getResources(), R.drawable.template);
        Bitmap idNumbar = findIdNumber(bitmap, template, Bitmap.Config.ARGB_8888);
        bitmap.recycle();
        if (idNumbar != null) {
            img_1.setImageBitmap(idNumbar);
        } else {
            return;
        }
    }

    public native Bitmap findIdNumber(Bitmap bitmap, Bitmap tpl, Bitmap.Config argb8888);
}
