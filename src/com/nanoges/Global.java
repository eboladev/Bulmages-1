package com.nanoges;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.location.LocationListener;
import android.location.Location;

public class Global {
	public static CommentsDataSource datasource;
	public static LocationListener locationListener;
	public static Location lastlocation;
	// Database fields
	public static SQLiteDatabase database;
	public static MySQLiteHelper dbHelper;
	
	public Global(Context context) {
		dbHelper = new MySQLiteHelper(context);
		database = dbHelper.getWritableDatabase();
	}
	
}
