package com.example.activitystatistics;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;
import android.os.Environment;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

import androidx.appcompat.app.AppCompatActivity;

import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private EditText youtubeLinkEditText;
    private Spinner formatSpinner;
    private Button downloadButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        youtubeLinkEditText = findViewById(R.id.youtubeLink);
        formatSpinner = findViewById(R.id.formatSpinner);
        downloadButton = findViewById(R.id.downloadButton);

        if (youtubeLinkEditText == null) {
            Log.e("MainActivity", "youtubeLinkEditText is null");
        }
        if (formatSpinner == null) {
            Log.e("MainActivity", "formatSpinner is null");
        }
        if (downloadButton == null) {
            Log.e("MainActivity", "downloadButton is null");
        }

        String link = youtubeLinkEditText.getText() != null ? youtubeLinkEditText.getText().toString().trim() : null;
        if (link == null) {
            Log.e("MainActivity", "Link is null");
        }


        // Налаштування Spinner з форматами
        List<String> formats = Arrays.asList(".mp4", ".webm", ".mkv", ".mp3", ".aac", ".m4a");
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item, formats);
        formatSpinner.setAdapter(adapter);

        // Дія при натисканні кнопки завантаження
        downloadButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String link = youtubeLinkEditText.getText() != null ? youtubeLinkEditText.getText().toString().trim() : "";
                String format = formatSpinner.getSelectedItem() != null ? formatSpinner.getSelectedItem().toString() : "";

                // Додайте логування для діагностики
                Log.d("MainActivity", "Link: " + link);
                Log.d("MainActivity", "Format: " + format);

                if (link.isEmpty()) {
                    Toast.makeText(MainActivity.this, "Please enter a YouTube link", Toast.LENGTH_SHORT).show();
                } else if (format.isEmpty()) {
                    Toast.makeText(MainActivity.this, "Please select a format", Toast.LENGTH_SHORT).show();
                } else {
                    downloadVideo(link, format);
                }
            }
        });
    }


    private void downloadVideo(String link, String format) {

        String command = "yt-dlp -f " + format + " -o '/sdcard/Download/video_" + System.currentTimeMillis() + format + "' " + link;

        try {
            Process process = Runtime.getRuntime().exec(command);

            int exitCode = process.waitFor();
            if (exitCode == 0) {
                runOnUiThread(() -> Toast.makeText(MainActivity.this, "Download successful", Toast.LENGTH_SHORT).show());
            } else {
                runOnUiThread(() -> Toast.makeText(MainActivity.this, "Download failed", Toast.LENGTH_SHORT).show());
            }
        } catch (IOException | InterruptedException e) {
            runOnUiThread(() -> Toast.makeText(MainActivity.this, "Error: " + e.getMessage(), Toast.LENGTH_SHORT).show());
        }
    }




}
