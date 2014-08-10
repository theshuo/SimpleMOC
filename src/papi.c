#include"SimpleMOC_header.h"

#ifdef PAPI

// initialize papi with one thread first
void papi_serial_init(void)
{
	if ( PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT){
		fprintf(stderr, "PAPI library init error!\n");
		exit(1);
	}

	if (( PAPI_thread_init((long unsigned int (*)(void))
					pthread_self )) != PAPI_OK){
		PAPI_perror("PAPI_thread_init");
		exit(1);
	}
}

void counter_init( int *eventset, int *num_papi_events, Input I )
{
	char error_str[PAPI_MAX_STR_LEN];
	int stat;

	int * events;
    
    int thread = omp_get_thread_num();

    // Command line event
    if( I.papi_event_set == -1){
        *num_papi_events = 1;
		events = (int *) malloc( *num_papi_events * sizeof(int));
        PAPI_event_name_to_code( I.event_name, &events[0]);
    }
	// FLOPS
	if( I.papi_event_set == 0 )
	{
		*num_papi_events = 1;
		events = (int *) malloc( *num_papi_events * sizeof(int));
		events[0] = PAPI_FP_OPS;
		//events[1] = PAPI_TOT_CYC;
	}
	// Bandwidth
	if( I.papi_event_set == 1 )
	{
		*num_papi_events = 2;
		events = (int *) malloc( *num_papi_events * sizeof(int));
		events[0] = PAPI_L3_TCM;
		events[1] = PAPI_TOT_CYC;
	}
	// CPU Stall Reason
	if( I.papi_event_set == 2 )
	{
		*num_papi_events = 4;
		events = (int *) malloc( *num_papi_events * sizeof(int));
		int EventCode;
		char * event1 = "RESOURCE_STALLS:ANY";
		char * event2 = "RESOURCE_STALLS:SB";
		char * event3 = "RESOURCE_STALLS:RS";
		char * event4 = "RESOURCE_STALLS2:OOO_RSRC";
		PAPI_event_name_to_code( event1, &EventCode );
		events[0] = EventCode;	
		PAPI_event_name_to_code( event2, &EventCode );
		events[1] = EventCode;	
		PAPI_event_name_to_code( event3, &EventCode );
		events[2] = EventCode;	
		PAPI_event_name_to_code( event4, &EventCode );
		events[3] = EventCode;	
	}
	// CPU Stall Percentage
	if( I.papi_event_set == 3 )
	{
		*num_papi_events = 2;
		events = (int *) malloc( *num_papi_events * sizeof(int));
		int EventCode;
		char * event1 = "RESOURCE_STALLS:ANY";
		char * event2 = "PAPI_TOT_CYC";
		PAPI_event_name_to_code( event1, &EventCode );
		events[0] = EventCode;	
		PAPI_event_name_to_code( event2, &EventCode );
		events[1] = EventCode;	
	}
	// Memory Loads
	if( I.papi_event_set == 4 )
	{
		*num_papi_events = 4;
		events = (int *) malloc( *num_papi_events * sizeof(int));
		int EventCode;
		char * event1 = "MEM_LOAD_UOPS_RETIRED";
		char * event2 = "MEM_LOAD_UOPS_RETIRED:L1_HIT";
		char * event3 = "MEM_LOAD_UOPS_RETIRED:L2_HIT";
		char * event4 = "MEM_LOAD_UOPS_RETIRED:L3_HIT";
		PAPI_event_name_to_code( event1, &EventCode );
		events[0] = EventCode;	
		PAPI_event_name_to_code( event2, &EventCode );
		events[1] = EventCode;	
		PAPI_event_name_to_code( event3, &EventCode );
		events[2] = EventCode;	
		PAPI_event_name_to_code( event4, &EventCode );
		events[3] = EventCode;	
	}
	// LLC Miss Rate
	if( I.papi_event_set == 5 )
	{
		*num_papi_events = 2;
		events = (int *) malloc( *num_papi_events * sizeof(int));
		events[0] = PAPI_L3_TCM;
		events[1] = PAPI_L3_TCA;
	}
	// Branch MisPrediction
	if( I.papi_event_set == 6 )
	{
		*num_papi_events = 3;
		events = (int *) malloc( *num_papi_events * sizeof(int));
		events[0] = PAPI_BR_MSP;
		events[1] = PAPI_BR_CN;
		events[2] = PAPI_BR_PRC;
	}
	// TLB Misses
	if( I.papi_event_set == 7 )
	{
		*num_papi_events = 4;
		events = (int *) malloc( *num_papi_events * sizeof(int));
		int EventCode;
		char * event1 = "perf::DTLB-LOADS";
		char * event2 = "perf::DTLB-LOAD-MISSES";
		char * event3 = "perf::DTLB-STORES";
		char * event4 = "perf::DTLB-STORE-MISSES";
		PAPI_event_name_to_code( event1, &EventCode );
		events[0] = EventCode;	
		PAPI_event_name_to_code( event2, &EventCode );
		events[1] = EventCode;	
		PAPI_event_name_to_code( event3, &EventCode );
		events[2] = EventCode;	
		PAPI_event_name_to_code( event4, &EventCode );
		events[3] = EventCode;	
	}

	/////////////////////////////////////////////////////////////////////////
	//                        PAPI EVENT LOADING
	/////////////////////////////////////////////////////////////////////////
	// Users should not need to alter anything within this section

	if ( (stat= PAPI_create_eventset(eventset)) != PAPI_OK)
	{
		PAPI_perror("PAPI_create_eventset");
		exit(1);
	}

    char event_name[PAPI_MAX_STR_LEN];
	for( int i = 0; i < *num_papi_events; i++ )
	{
		if ((stat=PAPI_add_event(*eventset,events[i])) != PAPI_OK)
		{
			PAPI_perror("PAPI_add_event");
			exit(1);
		}
        /*
        PAPI_event_code_to_name(events[i],event_name); 
        if( thread == 0)
            printf("Added event %s\n",event_name);
        */
	}

	if ((stat=PAPI_start(*eventset)) != PAPI_OK)
	{
		PAPI_perror("PAPI_start");
		exit(1);
	}

}


// Stops the papi counters and prints results
void counter_stop( int * eventset, int num_papi_events, Input * I )
{
	int * events = malloc(num_papi_events * sizeof(int));
	int n = num_papi_events;
	PAPI_list_events( *eventset, events, &n );
	PAPI_event_info_t info;

	long_long * values = malloc( num_papi_events * sizeof(long_long));
	PAPI_stop(*eventset, values);
	int thread = omp_get_thread_num();
	int nthreads = omp_get_num_threads();

	static long LLC_cache_miss = 0;
	static long total_cycles = 0;
	static long FLOPS = 0;
	static long stall_any = 0;
	static long stall_SB = 0;
	static long stall_RS = 0;
	static long stall_OO = 0;
	static long tlb_load = 0;
	static long tlb_load_m = 0;
	static long tlb_store = 0;
	static long tlb_store_m = 0;

    #pragma omp master
    {
        printf("Allocating vals accum array\n");
        I->vals_accum = malloc( num_papi_events * sizeof(long long));
        for(int i=0; i < num_papi_events ; i ++)
            I->vals_accum[i] = 0;
    }
    #pragma omp barrier

	#pragma omp critical (papi)
	{
		printf("Thread %d\n", thread);
		for( int i = 0; i < num_papi_events; i++ )
		{
            I->vals_accum[i] += values[i];
			PAPI_get_event_info(events[i], &info);
			printf("%-15lld\t%s\t%s\n", values[i],info.symbol,info.long_descr);
			if( strcmp(info.symbol, "PAPI_L3_TCM") == 0 )
				LLC_cache_miss += values[i];
			if( strcmp(info.symbol, "PAPI_TOT_CYC") == 0 )
				total_cycles += values[i];
			if( strcmp(info.symbol, "PAPI_FP_OPS") == 0 )
				FLOPS += values[i];
			if( strcmp(info.symbol, "RESOURCE_STALLS:ANY") == 0 )
				stall_any += values[i];
			if( strcmp(info.symbol, "RESOURCE_STALLS:SB") == 0 )
				stall_SB += values[i];
			if( strcmp(info.symbol, "RESOURCE_STALLS:RS") == 0 )
				stall_RS += values[i];
			if( strcmp(info.symbol, "RESOURCE_STALLS2:OOO_RSRC") == 0 )
				stall_OO += values[i];
			if( strcmp(info.symbol, "perf::DTLB-LOADS") == 0 )
				tlb_load += values[i];
			if( strcmp(info.symbol, "perf::DTLB-LOAD-MISSES") == 0 )
				tlb_load_m += values[i];
			if( strcmp(info.symbol, "perf::DTLB-STORES") == 0 )
				tlb_store += values[i];
			if( strcmp(info.symbol, "perf::DTLB-STORE-MISSES") == 0 )
				tlb_store_m += values[i];
		}
		free(events);
		free(values);	
	}
	{
		#pragma omp barrier
	}
	#pragma omp master
	{
        if( omp_get_num_threads() > 1){
            printf("Thread Totals:\n");
            for( int i = 0; i < num_papi_events; i++ )
            {
                PAPI_get_event_info(events[i], &info);
                printf("%-15lld\t%s\t%s\n", I->vals_accum[i],info.symbol,info.long_descr);
            }
        }
        free( I->vals_accum );

		border_print();
		center_print("PERFORMANCE SUMMARY", 79);
		border_print();
		long cycles = (long) (total_cycles / (double) nthreads);
		double bw = LLC_cache_miss*64./cycles*2.8e9/1024./1024./1024.;
		if( I->papi_event_set == 0 )
			printf("GFLOPs: %.3lf\n", FLOPS / (double) cycles * 2.8  );
		if( I->papi_event_set == 1 )
			printf("Bandwidth: %.3lf (GB/s)\n", bw);
		if( I->papi_event_set == 2 )
		{
			printf("%-30s %.2lf%%\n", "Store Buffer Full:",
					stall_SB / (double) stall_any * 100.);
			printf("%-30s %.2lf%%\n", "Reservation Station Full:",
					stall_RS / (double) stall_any * 100.);
			printf("%-30s %.2lf%%\n", "OO Pipeline Full:",
					stall_OO / (double) stall_any * 100.);
		}
		if( I->papi_event_set == 3 )
			printf("CPU Stalled Cycles: %.2lf%%\n",
					stall_any / (double) total_cycles * 100.);	
		if( I->papi_event_set == 7 )
		{
			printf("%-30s %.2lf%%\n", "Data TLB Load Miss Rate: ",
					tlb_load_m / (double) tlb_load * 100 );
			printf("%-30s %.2lf%%\n", "Data TLB Store Miss Rate: ",
					tlb_store_m / (double) tlb_store * 100 );
		}

		border_print();
	}
}

#endif
