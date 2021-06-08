# preg-init
Small program for starting and managing started processes and their lifetime etc. Usefull in docker envorinments.

Config syntax
-----------------

Most config options are optional

```

end-signal SIGNAME;

spawn "name" {
	file XXX;
	argument XXX;
	start-delay XXX;
	on-death rerun/none/stop [when-rc XXX];
	cwd XXX;
	proxy signals {
		signal SIGNAME [as SIGNAME];
	};
	proxy stdout {
		[prefix "..."];
		to "stdout/stderr/file_name";
	};
	proxy stderr {
		[prefix "..."];
		to "stdout/stderr/file_name";
	};	
};

spawn "..." { 
...
};

```

Example config in build/test.conf



