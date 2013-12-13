package com.example.cancerdiagnosis1;

import java.io.File;

import android.os.Environment;

public final class BaseAlbumDirFactory extends AlbumStorageDirFactory {

	// Standard storage location for digital camera files
	private static final String CAMERA_DIR = "/dcim/";

	@Override
	//albumName = MyPictures
	public File getAlbumStorageDir(String albumName) {
		return new File (Environment.getExternalStorageDirectory() + CAMERA_DIR + albumName);
	}
}
