using img_t = cimg_library::CImg<int>;

void sequentialwatermarking (std::string path_in, std::string w_path_in, std::string path_out,bool compute_avg) {
	struct dirent *photoDirent;
	DIR *photoDir;
	int n_images = 0;
    float proc_time = 0;
    float save_time = 0;
    float read_time = 0;

    //std::cout<<"sequentialwatermarking "<<std::endl;
	photoDir = opendir (path_in.c_str());
	if (photoDir == NULL) {
		std::cout<<"Cannot open directory "<<path_in<<"\n";
	}

	auto start_tc=std::chrono::high_resolution_clock::now();
	img_t watermark(w_path_in.c_str());
	while ((photoDirent = readdir(photoDir)) != NULL) {
		if (strcmp(photoDirent->d_name, ".") != 0 && strcmp(photoDirent->d_name, "..") != 0 ) 
		{
			auto start = std::chrono::high_resolution_clock::now();
			std::string file_name(path_in + photoDirent->d_name);
			std::string result_name(path_out + photoDirent->d_name);
			img_t img(file_name.c_str());
			auto elapsed=std::chrono::high_resolution_clock::now()-start;
			auto msec=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
			read_time+=(float) msec;

			start=std::chrono::high_resolution_clock::now();
			apply_watermark(img, watermark, compute_avg);
			elapsed=std::chrono::high_resolution_clock::now()-start;
			msec=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
			proc_time+=(float) msec;

			start=std::chrono::high_resolution_clock::now();
			img.save_jpeg(result_name.c_str(), 100);
			elapsed=std::chrono::high_resolution_clock::now()-start;
			msec=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();            
			save_time+=(float) msec;

			n_images++;
		}
	}
	closedir(photoDir);
	auto elapsed_tc=std::chrono::high_resolution_clock::now()-start_tc;
	auto msec_tc=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_tc).count();
	//std::cout<<"processed "<<std::endl;
	//std::cout<<n_images<<" ";
	std::cout<<read_time<<" "<<proc_time<<" "<<save_time<<" ";
	std::cout<<msec_tc<<std::endl;
	}