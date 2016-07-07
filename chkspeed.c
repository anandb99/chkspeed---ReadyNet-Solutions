/***************************************************************************

To compile & run this code on router, use folowing command:

/opt/buildroot-gcc463/usr/bin/mipsel-linux-gcc -I /home/anand/Downloads/Flying\ Voice/RT288x_SDK/source/user/curl/include -L /home/anand/Downloads/Flying\ Voice/RT288x_SDK/source/user/curl/lib -o chkspeed_router_05JUL2016_0114PM chkspeed.c -lcurl -lm -std=c99

***************************************************************************/
 
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * Show transfer timing info after upload and download completes.
 * </DESC>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/socket.h>

#define pi 3.14159265358979323846


double deg2rad(double);
double rad2deg(double);
double distance(double, double, double, double, char); 


double distance(double lat1, double lon1, double lat2, double lon2, char unit) 
{
  	  double theta, dist;
	  theta = lon1 - lon2;
	  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
	  dist = acos(dist);
	  dist = rad2deg(dist);
	  dist = dist * 60 * 1.1515;
	  switch(unit)
	 {
	    case 'M':
	 	     break;
	    case 'K':
	     	 dist = dist * 1.609344;
	      	break;
	    case 'N':
	      	dist = dist * 0.8684;
	      	break;
	  }
	  return (dist);
}

double deg2rad(double deg) 
{
	  return (deg * pi / 180);
}

double rad2deg(double rad) 
{
	  return (rad * 180 / pi);
}


int main(int argc, char *argv[])
{
  CURL *curl_handle;
  CURLcode res;
  int prtall = 0, prtsep = 0, prttime = 0;
  double data_size = 5;
  double total_time = 1;
  char *appname = argv[0];

////////// Create random file for upload //////////

    FILE *fp_random ;
    int i_random, j_random ;
    long double noofr_random = powl(2,17)-powl(2,13);

    remove("random.ReadyNet");

    fp_random = fopen ( "random.ReadyNet", "wb" ) ;
    if ( fp_random == NULL )
    {
        printf ( "Unable to create file." ) ;
        exit ( 0 ) ;
    }

    for ( i_random = 0 ; i_random < noofr_random ; i_random++ )
    {
    	j_random = rand();
        fwrite ( &j_random, sizeof ( int ), 1, fp_random ) ;
        //printf ( "%d\t", j ) ;
    }

    fclose ( fp_random ) ;
    //printf ( "\nFile is created. Press any key to continue. \n\n" ) ;


////////// Adjust TCP Receive and Transmit Buffer //////////

 int result = 0;

 int sockfd, ordinary_sendbuff,ordinary_recvbuff;
 socklen_t optlen;

 sockfd = socket(AF_INET, SOCK_DGRAM, 0);
 if(sockfd == -1)
     printf("Error");

 // Get send buffer size
 optlen = sizeof(ordinary_sendbuff);
 res = getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &ordinary_sendbuff, &optlen);

 if(result == -1)
     printf("Error getsockopt one");
/*
 else
     printf("send buffer size = %d\n", ordinary_sendbuff);
*/

 // Set send buffer size
 //sendbuff = 98304;
 long double max_sendbuff = powl(2,31)-1;
 //printf("sets the send buffer to %Lf\n", max_sendbuff);
 res = setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &max_sendbuff, sizeof(max_sendbuff));

 if(result == -1)
     printf("Error setsockopt");


 // Get send buffer size
 optlen = sizeof(max_sendbuff);
 res = getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &max_sendbuff, &optlen);

 if(result == -1)
     printf("Error getsockopt two");
/*
 else
     printf("send buffer size = %Lf\n", max_sendbuff);
*/

////////////////////////////////////

// Get recv buffer size
 optlen = sizeof(ordinary_recvbuff);
 res = getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &ordinary_recvbuff, &optlen);

 if(result == -1)
     printf("Error getsockopt one");
/*
 else
     printf("send buffer size = %d\n", ordinary_recvbuff);
*/
 // Set recv buffer size
 //sendbuff = 98304;
 long double max_recvbuff = powl(2,31)-1;
 //printf("sets the send buffer to %Lf\n", max_recvbuff);
 res = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &max_recvbuff, sizeof(max_recvbuff));

 if(result == -1)
     printf("Error setsockopt");


 // Get recv buffer size
 optlen = sizeof(max_recvbuff);
 res = getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &max_recvbuff, &optlen);

 if(result == -1)
     printf("Error getsockopt two");
/*
 else
     printf("send buffer size = %Lf\n", max_recvbuff);
*/

////////// Download speedtest-config.xml //////////

    CURL *curl_config;
    FILE *fp_config;
    CURLcode res_config;

    remove("speedtest-config.xml");

    char *url_config = "http://www.speedtest.net/speedtest-config.php";
    char outfilename_config[FILENAME_MAX] = "speedtest-config.xml";

    curl_config=curl_easy_init();
                                                                                                                                                                                                                                                    
    if (curl_config)
    {   
        fp_config = fopen(outfilename_config,"wb");
        curl_easy_setopt(curl_config, CURLOPT_URL, url_config);
        curl_easy_setopt(curl_config,CURLOPT_USERAGENT,"Dark Secret Ninja/1.0");
        curl_easy_setopt(curl_config, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl_config, CURLOPT_WRITEDATA, fp_config);
        res_config = curl_easy_perform(curl_config);
        curl_easy_cleanup(curl_config);
        fclose(fp_config);
    }  

////////// Download speedtest-servers.xml //////////

    CURL *curl_servers;
    FILE *fp_servers;
    CURLcode res_servers;

    remove("speedtest-servers.xml");

    char *url_servers = "http://www.speedtest.net/speedtest-servers.php";
    char outfilename_servers[FILENAME_MAX] = "speedtest-servers.xml";

    curl_servers=curl_easy_init();
                                                                                                                                                                                                                                                    
    if (curl_servers)
    {   
        fp_servers = fopen(outfilename_servers,"wb");
        curl_easy_setopt(curl_servers, CURLOPT_URL, url_servers);
        curl_easy_setopt(curl_servers,CURLOPT_USERAGENT,"Dark Secret Ninja/1.0");
        curl_easy_setopt(curl_servers, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl_servers, CURLOPT_WRITEDATA, fp_servers);
        res_servers = curl_easy_perform(curl_servers);
        curl_easy_cleanup(curl_servers);
        fclose(fp_servers);
    }  
    //printf("\n Downloaded speedtest-servers.xml");

////////// Find clinet and server specific data //////////

    FILE * fp_speedtest_config;
    FILE * fp_speedtest_servers;

    char * line = NULL;
    char client_main_line[256],server_main_line[256];

    char  cli_lat[9],cli_lon[9],ser_lat[9],ser_lon[9],ser_URL[128],ser_close_URL[128];
    int cli_lat_loc = 0,cli_lon_loc = 0,ser_lat_loc = 0,ser_lon_loc = 0,ser_URL_loc = 0,ser_close_URL_loc = 0;

    size_t len = 0,read;

    double lat,lon,ser_temp_lat,ser_temp_lon,ser_close_lat,ser_close_lon,dist,min_dist=99999;
    int j=0,k=0,point_loc=0;
    char i = '\0';

//////////**********//////////

    fp_speedtest_config = fopen("speedtest-config.xml", "r");
    
    if (fp_speedtest_config == NULL)
    {
	printf("\n Unable to find speedtest-config.xml");
     	exit(EXIT_FAILURE);
    }

    if((read = getline(&line, &len, fp_speedtest_config)) != -1) {}
    if((read = getline(&line, &len, fp_speedtest_config)) != -1) {}

	if((read = getline(&line, &len, fp_speedtest_config)) != -1) 
	{
		j=0;
		point_loc=0;

		for(i=line[j],j=0; i!='\0'; j++)
		{
			i=line[j];
			client_main_line[j]=line[j];
			if(line[j]=='l' && line[j+1]=='a' && line[j+2]=='t' && line[j+3]=='=' && line[j+4]=='"')
			{
				//printf("\n\n Found lat \n\n");
				point_loc=j+5;
				while(line[point_loc]!='"')
				{
					cli_lat[cli_lat_loc]=line[point_loc];
					point_loc++;
					cli_lat_loc++;
				}
				cli_lat[cli_lat_loc]='\0';
				sscanf(cli_lat,"%lf",&lat);
				//printf("\n\n Client lat: %lf \n\n", lat);
			}
			else if(line[j]=='l' && line[j+1]=='o' && line[j+2]=='n' && line[j+3]=='=' && line[j+4]=='"')
			{
				//printf("\n\n Found lon \n\n");
				point_loc=j+5;
				while(line[point_loc]!='"')
				{
					cli_lon[cli_lon_loc]=line[point_loc];
					point_loc++;
					cli_lon_loc++;
				}
				cli_lon[cli_lon_loc]='\0';
				sscanf(cli_lon,"%lf",&lon);
				//printf("\n\n Client lon: %lf \n\n", lon);
			}
		}
		client_main_line[j]='\0';
	}

    fclose(fp_speedtest_config);
    remove("speedtest-config.xml");

///////////////////////////////////////////////////////////////////////////////////////////////////////

    fp_speedtest_servers = fopen("speedtest-servers.xml", "r");
   
    if (fp_speedtest_servers == NULL)
        exit(EXIT_FAILURE);

    if((read = getline(&line, &len, fp_speedtest_servers)) != -1) {}
    if((read = getline(&line, &len, fp_speedtest_servers)) != -1) {}

	if((read = getline(&line, &len, fp_speedtest_servers)) != -1) 
	{
		j=0;
		point_loc=0;
		//line_num++;

		for(i=line[j],j=0; i!='\0'; j++)
		{
			i=line[j];

			if(line[j]=='u' && line[j+1]=='r' && line[j+2]=='l' && line[j+3]=='=' && line[j+4]=='"')
			{
				point_loc=j+5;
				while(line[point_loc]!='"')
				{
					ser_URL[ser_URL_loc]=line[point_loc];
					point_loc++;
					ser_URL_loc++;
				}
				ser_URL[ser_URL_loc]='\0';
				//printf("\n server URL : %s", ser_URL);
				ser_URL_loc=0;
			}
			else 

			if(line[j]=='l' && line[j+1]=='a' && line[j+2]=='t' && line[j+3]=='=' && line[j+4]=='"')
			{
				//printf("\n\n Found server lat \n\n");
				point_loc=j+5;
				while(line[point_loc]!='"')
				{
					ser_lat[ser_lat_loc]=line[point_loc];
					point_loc++;
					ser_lat_loc++;
				}
				ser_lat[ser_lat_loc]='\0';
				sscanf(ser_lat,"%lf",&ser_temp_lat);
				//printf("\n First server lat: %lf", ser_temp_lat);
				ser_lat_loc=0;
			}
			else if(line[j]=='l' && line[j+1]=='o' && line[j+2]=='n' && line[j+3]=='=' && line[j+4]=='"')
			{
				//printf("\n\n Found server lon \n\n");
				point_loc=j+5;
				while(line[point_loc]!='"')
				{
					ser_lon[ser_lon_loc]=line[point_loc];
					point_loc++;
					ser_lon_loc++;
				}
				ser_lon[ser_lon_loc]='\0';
				sscanf(ser_lon,"%lf",&ser_temp_lon);
				//printf("\n First server lon: %lf", ser_temp_lon);
				ser_lon_loc=0;
			}
	
			min_dist = distance(lat, lon, ser_temp_lat, ser_temp_lon, 'M');
		}
	}

	while((read = getline(&line, &len, fp_speedtest_servers)) != -1) 
	{
		j=0;
		point_loc=0;
		//line_num++;
		/*
		URL_found=0;
		lat_found=0;
		lon_found=0;
*/
		for(i=line[j],j=0; i!='\0'; j++)
		{
			i=line[j];

			if(line[j]=='l' && line[j+1]=='a' && line[j+2]=='t' && line[j+3]=='=' && line[j+4]=='"')
			{
				//printf("\n\n Found server lat \n\n");
				point_loc=j+5;
				while(line[point_loc]!='"')
				{
					ser_lat[ser_lat_loc]=line[point_loc];
					point_loc++;
					ser_lat_loc++;
				}
				ser_lat[ser_lat_loc]='\0';
				sscanf(ser_lat,"%lf",&ser_temp_lat);
				//printf("\n server lat: %lf", ser_temp_lat);
				ser_lat_loc=0;
				//lat_found=1;

			}
			else if(line[j]=='l' && line[j+1]=='o' && line[j+2]=='n' && line[j+3]=='=' && line[j+4]=='"')
			{
				//printf("\n\n Found server lon \n\n");
				point_loc=j+5;
				while(line[point_loc]!='"')
				{
					ser_lon[ser_lon_loc]=line[point_loc];
					point_loc++;
					ser_lon_loc++;
				}
				ser_lon[ser_lon_loc]='\0';
				sscanf(ser_lon,"%lf",&ser_temp_lon);
				//printf("\n server lon: %lf", ser_temp_lon);
				ser_lon_loc=0;
				//lon_found=1;
			}
		}

			dist = distance(lat, lon, ser_temp_lat, ser_temp_lon, 'M');
			if(dist<min_dist)
			{
				min_dist=dist;
				ser_close_lat=ser_temp_lat;
				ser_close_lon=ser_temp_lon;
				//close_line = line_num;
		
				for(k=0;k<read-1;k++)
					server_main_line[k]=line[k];
				server_main_line[k++]='\0';
				dist = 99999;
			}
	}

    fclose(fp_speedtest_servers);
    remove("speedtest-servers.xml");

////////// Testing Download Speed /////////

	for(j=0,k=13;server_main_line[k]!='"';j++,k++)
		ser_close_URL[j]=server_main_line[k];
	ser_close_URL[j]='\0';
	//printf("\n Closest Server : %s",ser_close_URL);
	ser_close_URL[j-10]='\0';
	ser_close_URL_loc = j-10;
	//printf("\n Trimmed Closest Server : %s",ser_close_URL);
	char temp_ser_URL[128];
	temp_ser_URL[0]='\0';
	//printf("\n ser_close_url : %s",ser_close_URL);
	strcpy(temp_ser_URL,ser_close_URL);
	//printf("\n temp Closest Server : %s",temp_ser_URL);
	
	
    CURL *curl_TEMP;
    FILE *fp_TEMP;
    CURLcode res_TEMP;

    double download_file_size[10]={245388,505544,1118012,1986284,4468241,7907740,12407926,17816816,24262167,31625365};
    double data_download_size=0;
    char url_TEMP[10][128]={{'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'}};

    //strcpy(url_TEMP,temp_ser_URL);
    //int url_TEMP_len = strlen(url_TEMP);
    //printf("\n url_TEMP : %s",url_TEMP);
    char outfilename_TEMP[FILENAME_MAX];    
	

	char temp_file[10][20]={{"random350x350.jpg"},{"random500x500.jpg"},{"random750x750.jpg"},{"random1000x1000.jpg"},{"random1500x1500.jpg"},{"random2000x2000.jpg"},{"random2500x2500.jpg"},{"random3000x3000.jpg"},{"random3500x3500.jpg"},{"random4000x4000.jpg"}};
	//char temp_file_name[20];
	double val=0,time_download=0,data_download=0;

	clock_t start,end;
	double cpu_time_download;
	start = clock();

	for(j=0;j<10;j++)
	{
		strcpy(url_TEMP[j],temp_ser_URL);
		 //url_TEMP_len = strlen(url_TEMP[j]);
		//url_TEMP[j][ url_TEMP_len]='\0';
		strcat(url_TEMP[j],temp_file[j]);
		//printf("\n Final url_TEMP : %s",url_TEMP[j]);		
		
		//printf("\n Download File name : %s",temp_file[j]);

		//char * temp_file1="random350x350.jpg";

		strcpy(outfilename_TEMP,temp_file[j]);
		//strcat(temp_ser_URL,temp_file_name);
		//printf("\n cat Closest Server : %s",temp_ser_URL);
	  
		//for(int i=1;i<=4;i++)
		//{
		    //char post[1];
		    //sprintf(post,"%d",i);
		    //strcat(outfilename_TEMP,post);

		    //printf("\n Downloading %s",outfilename_TEMP);

		    curl_TEMP=curl_easy_init();
				                                                                                                                                                                                                                                 
		    if (curl_TEMP)
		    {   
			fp_TEMP = fopen(outfilename_TEMP,"wb");
			curl_easy_setopt(curl_TEMP, CURLOPT_URL, url_TEMP[j]);
			curl_easy_setopt(curl_TEMP,CURLOPT_USERAGENT,"Dark Secret Ninja/1.0");
			curl_easy_setopt(curl_TEMP, CURLOPT_WRITEFUNCTION, NULL);
			curl_easy_setopt(curl_TEMP, CURLOPT_WRITEDATA, fp_TEMP);
			res_TEMP = curl_easy_perform(curl_TEMP);
			
			if(CURLE_OK == res_TEMP) 
			{
    				val=0;
				res_TEMP = curl_easy_getinfo(curl_TEMP, CURLINFO_SIZE_DOWNLOAD, &val);
				if((CURLE_OK == res_TEMP) && (val>0))
				{
				 	data_download = data_download + val;   	
				}

    				val=0;
				res_TEMP = curl_easy_getinfo(curl_TEMP, CURLINFO_TOTAL_TIME, &val);
				if((CURLE_OK == res_TEMP) && (val>0))
    				{
					//printf("\n val: %f",val);
					time_download = time_download + val;
    				}

				data_download_size = data_download_size + download_file_size[j];

			}
			curl_easy_cleanup(curl_TEMP);
			fclose(fp_TEMP);
			remove(temp_file[j]);

			if(time_download >= 25)
			{
				goto outside_Testing_Download_Speed;
			}
		    } 
		//}
	}

outside_Testing_Download_Speed:

	end = clock();
	cpu_time_download = ((double) (end-start)) / CLOCKS_PER_SEC;
	cpu_time_download = cpu_time_download/10;
	
	//printf("\n\n Download done \n\n");

////////// Testing Upload Speed /////////

  CURL *curl_upload;
  CURLcode res_upload;
  struct stat file_info_upload;
  double speed_upload, total_time_upload;
  FILE *fd_upload;

	//char outfilename_TEMP[FILENAME_MAX];    
	//char temp_file[10][20]={{"350"},{"500"},{"750"},{"1000"},{"1500"},{"2000"},{"2500"},{"3000"},{"3500"},{"4000"}};
	//char temp_file_name[20];
	double time_upload=0,data_upload=0,upload_time=0;

	double data_upload_size=0;

	strcat(ser_close_URL,"upload.php");
	//printf("\n upload URL : %s \n\n",ser_close_URL);

	clock_t start_upload,end_upload;
	double cpu_time_upload;
	start_upload = clock();
/*
	for(k=0;k<25;k++)
	{
*/
		for(j=0;j<512;j++)
			{
				//strcpy(temp_file_name,"random");
				//strcat(temp_file_name,temp_file[j]);
				//strcat(temp_file_name,"x");
				//strcat(temp_file_name,temp_file[j]);
				//strcat(temp_file_name,".jpg");
				//printf("\n Upload File name : %s \n",temp_file_name);
				//char * temp_file1="random350x350.jpg";


			//fd_upload = fopen("random500.jpg", "rb"); //* open file to upload 
			fd_upload = fopen("random.ReadyNet", "rb"); //* open file to upload 			  

			if(!fd_upload)
			  {
			    printf("\n\n cant upload! \n\n");
			    return 1; // can't continue 
			  }

			 //to get the file size 
			 // if(fstat(fileno(fd_upload), &file_info_upload) != 0) 
			 // {
			//return 1; // can't continue 
			 // }
				

			  curl_upload = curl_easy_init();

			  if(curl_upload) 
			  {
			    // upload to this place 
			    curl_easy_setopt(curl_upload, CURLOPT_URL,ser_close_URL);
			    curl_easy_setopt(curl_upload, CURLOPT_USERAGENT,"Dark Secret Ninja/1.0");

			    //* tell it to "upload" to the URL 
			    curl_easy_setopt(curl_upload, CURLOPT_UPLOAD, 1L);

			    //* set where to read from (on Windows you need to use READFUNCTION too) 
			    curl_easy_setopt(curl_upload, CURLOPT_READDATA, fd_upload);

			    //* and give the size of the upload (optional) 
			    //curl_easy_setopt(curl_upload, CURLOPT_INFILESIZE_LARGE,(curl_off_t)file_info_upload.st_size);

			    //* enable verbose for easier tracing 
			    //curl_easy_setopt(curl_upload, CURLOPT_VERBOSE, 1L);

			    res_upload = curl_easy_perform(curl_upload);
			    //* Check for errors 
			    if(res_upload != CURLE_OK) 
			    {
			      //fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res_upload));
			    }
			    else 
			    {
			      //* now extract transfer info 
			      //curl_easy_getinfo(curl_upload, CURLINFO_SPEED_UPLOAD, &speed_upload);
			      curl_easy_getinfo(curl_upload, CURLINFO_TOTAL_TIME, &total_time_upload);

			      //fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n",speed_upload, total_time_upload);
			      upload_time=upload_time+total_time_upload;

				val=0;
				curl_easy_getinfo(curl_upload, CURLINFO_SIZE_UPLOAD, &val);
				data_upload = data_upload + val;  
				//printf("\n Data upload this time: %f",val);
				data_upload_size = data_upload_size + 491772; // file size:491520, upload size:491772 	
				
			    }
			    //* always cleanup 
			    curl_easy_cleanup(curl_upload);
				//printf("\n\n upload successfull ! \n\n");
			  }
	
			if(upload_time >= 25)
			{
				goto outside_Testing_Upload_Speed;
			}

		  }
		  //remove(temp_file_name);

	//}

outside_Testing_Upload_Speed:

	remove("random.ReadyNet");

	end_upload = clock();
	cpu_time_upload = ((double) (end_upload-start_upload)) / CLOCKS_PER_SEC;
	cpu_time_upload = cpu_time_upload/72;

////////// Showing Results /////////

	printf("\n\n-------------------------------------------------");
	printf("\n Client Latitude: %lf", lat);
	printf("\n Client Longitude: %lf", lon);
	printf("\n Closeset Server Latitude: %lf", ser_close_lat);
	printf("\n Closeset Server Longitude: %lf", ser_close_lon);
	printf("\n Distance to Closeset Server: %lf miles", min_dist);
	printf("\n-------------------------------------------------");

	printf("\n Clinet Details: \n %s",client_main_line);
	printf("\n Server Details: \n %s",server_main_line);


	for(j=0,k=13;server_main_line[k]!='"';j++,k++)
		ser_close_URL[j]=server_main_line[k];
	ser_close_URL[j]='\0';
	//printf("\n Closest Server : %s",ser_close_URL);
	ser_close_URL[j-10]='\0';
	ser_close_URL_loc = j-10;
	//printf("\n Trimmed Closest Server : %s",ser_close_URL);
	printf("\n-------------------------------------------------");

	printf("\n Download time: %0.2f sec.", time_download);
	//printf("\n Total download time: %0.3f sec.", cpu_time_download);
	//printf("\n Download speed: %0.3f Mbps.", data_download*8 /1024/1024/cpu_time_download);
	//printf("\n Data downloaded: %0.0f bytes.", data_download);
	printf("\n Data downloaded size: %0.0f bytes.", data_download_size);
	//data_download=102343483;
	// factor 1.0005
	printf("\n Download speed: %0.2f Mbps.", data_download_size*8/1000/1000/time_download);

	//printf("Data downloaded: %0.0f Kbytes.\n", data/1024);
	//printf("Data downloaded: %0.0f Mbytes.\n", data/1024/1024);
	printf("\n-------------------------------------------------");
	//printf("\n Total Upload time: %0.3f sec.", cpu_time_upload);
	printf("\n Upload time: %0.2f sec.", upload_time);
	//printf("\n Data uploaded: %0.3f.", data_upload);
	printf("\n Data uploaded size: %0.0f bytes.", data_upload_size);
	//data_upload=25277200;
	printf("\n Upload speed: %0.2f Mbps.", data_upload_size*8/1000/1000/upload_time);
	//printf("\n Upload speed: %0.2f Mbps.", data_upload*8 /1024/1024/cpu_time_upload);
	printf("\n-------------------------------------------------");

    if (line)
        free(line);
    printf("\n\n");
    exit(EXIT_SUCCESS);
    return 0;
}

