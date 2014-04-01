package com.projectfirestone;

import com.parse.Parse;
import com.parse.ParseACL;

import android.app.Application;

public class ApplicationProjectFirestone extends Application {
	
	@Override
	public void onCreate() {
		super.onCreate();
		Parse.initialize(this, "rkhsp24ClVFLBzRb0Ankum6rikte0WHeDYMR8Azu", "nxosGxlCACW8NdUky8Su6FTNr1oLsqCgFs2DBuLa");		
		ParseACL defaultACL = new ParseACL();
		// Optionally enable public read access.
		defaultACL.setPublicReadAccess(true);
		defaultACL.setPublicWriteAccess(true);
		ParseACL.setDefaultACL(defaultACL, true);	
	}

}
