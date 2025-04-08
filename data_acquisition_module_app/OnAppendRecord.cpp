void CAppendRecordDlg::OnAppendRecord() 
{
	UpdateData( TRUE );

   XC_CONFIGURE_STRUCTURE_TYPE cfg;
	XC_TIMER_EVENT_STRUCTURE_TYPE rec;

	struct db_info
	{
		char *source;
      char *table;
   };
   
   static struct db_info db_options[] =
   {
      { "MS Access Database",   "x_access" },
      { "dBASE Files",          "x_dbase4"  },
      { "Excel Files",          "x_excel"  },
      { "Visual Foxpro Database", "x_foxpro" }
   };

	strcpy( cfg.database_odbc_data_source_name, db_options[m_database_type].source );
	strcpy( cfg.database_table_name, db_options[m_database_type].table );

   /* unsigned short */ rec.timing_device_field                   =  7 ;
   /* unsigned long  */ rec.record_number_for_device_field        = 77 ;
   /* unsigned short */ rec.timer_channel_for_device_field        =  8 ;
   /* char [20]      */ strcpy( rec.total_user_string_field, "total_user" );
   /* char [16][20]  */ strcpy( rec.user_string_array_field[0], "user_string_0" );
   /* char [20]      */ strcpy( rec.bib_string_field, "bib_string" );
   /* char [16]      */ strcpy( rec.timer_time_string_field, "timer_time" );
   /* char [16]      */ strcpy( rec.pc_time_string_field, "pc_time" );

   for ( int i = 1; i < 16; ++i )
      rec.user_string_array_field[i][0] = '\0';
	
	append_record_to_database( &cfg, &rec );
}
