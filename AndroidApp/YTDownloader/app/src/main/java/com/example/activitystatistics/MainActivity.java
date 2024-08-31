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
        if (link == null || link.isEmpty()) {
            Toast.makeText(MainActivity.this, "Invalid link", Toast.LENGTH_SHORT).show();
            Log.e("MainActivity", "downloadVideo: Link is null or empty");
            return;
        }

        if (format == null || format.isEmpty()) {
            Toast.makeText(MainActivity.this, "Invalid format", Toast.LENGTH_SHORT).show();
            Log.e("MainActivity", "downloadVideo: Format is null or empty");
            return;
        }

        OkHttpClient client = new OkHttpClient();
        Request request = new Request.Builder()
                .url(link)
                .build();

        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                runOnUiThread(() -> {
                    Toast.makeText(MainActivity.this, "Download failed: " + e.getMessage(), Toast.LENGTH_SHORT).show();
                    Log.e("MainActivity", "Download failed: " + e.getMessage());
                });
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (response == null || !response.isSuccessful()) {
                    runOnUiThread(() -> {
                        Toast.makeText(MainActivity.this, "Download failed: " + (response != null ? response.message() : "Unknown error"), Toast.LENGTH_SHORT).show();
                        Log.e("MainActivity", "Download failed: response is null or unsuccessful");
                    });
                    return;
                }

                // Save video to the file system
                File path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS);
                File file = new File(path, "video_" + System.currentTimeMillis() + format);

                try (InputStream inputStream = response.body().byteStream();
                     FileOutputStream outputStream = new FileOutputStream(file)) {

                    byte[] buffer = new byte[4096];
                    int bytesRead;
                    while ((bytesRead = inputStream.read(buffer)) != -1) {
                        outputStream.write(buffer, 0, bytesRead);
                    }

                    runOnUiThread(() -> {
                        Toast.makeText(MainActivity.this, "Downloaded to: " + file.getAbsolutePath(), Toast.LENGTH_SHORT).show();
                        Log.d("MainActivity", "Video downloaded successfully");
                    });
                } catch (IOException e) {
                    runOnUiThread(() -> {
                        Toast.makeText(MainActivity.this, "Error saving video: " + e.getMessage(), Toast.LENGTH_SHORT).show();
                        Log.e("MainActivity", "Error saving video: " + e.getMessage());
                    });
                }
            }
        });
    }



}
