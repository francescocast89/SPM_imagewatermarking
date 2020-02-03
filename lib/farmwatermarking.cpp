using img_t = cimg_library::CImg<int>;

task_queue <std::string> tq;

std::vector<float> read_time_par;
std::vector<float> proc_time_par;
std::vector<float> save_time_par;
//std::vector<int> n_images_par;
std::string path_in_par;
std::string eos_str("EOS");

void source(int par_deg){
    struct dirent *photoDirent;
    DIR *photoDir;
    photoDir = opendir (path_in_par.c_str());

    if (photoDir == NULL) {
        std::cout<<"Cannot open directory "<<path_in_par<<std::endl;
    }
    while ((photoDirent = readdir(photoDir)) != NULL) {
        if (strcmp(photoDirent->d_name, ".") != 0 && strcmp(photoDirent->d_name, "..") != 0 ) 
        {
            tq.insert_task(photoDirent->d_name);
        }
    }
    for(int j = 0; j <= par_deg; j++)
    	tq.insert_task(eos_str);
    closedir(photoDir);
}

void worker(int i, img_t watermark, std::string path_out,bool compute_avg){
    while (true){ 	
        std::string file_name = tq.get_task();
        if(file_name!=eos_str){

        	auto start_r=std::chrono::high_resolution_clock::now();
        	img_t img((path_in_par + file_name).c_str());
            auto elapsed_r=std::chrono::high_resolution_clock::now()-start_r;
        	auto msec_r=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_r).count();
			read_time_par[i]+=msec_r;

            auto start_w=std::chrono::high_resolution_clock::now();
            apply_watermark(img, watermark, compute_avg);
            auto elapsed_w=std::chrono::high_resolution_clock::now()-start_w;
        	auto msec_w=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_w).count();
        	proc_time_par[i]+=msec_w;

            //n_images_par[i]++;
            try{

            	auto start_s=std::chrono::high_resolution_clock::now();
                img.save_jpeg((path_out+file_name).c_str(),100);
                auto elapsed_s=std::chrono::high_resolution_clock::now()-start_s;
        		auto msec_s=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_s).count();
        		save_time_par[i]+=msec_s;

            }catch(cimg_library::CImgIOException ex){
                std::cerr<<"Error occurred while saving "<<((path_out+file_name).c_str())<<" in worker "<<i<<std::endl;
            }
        }else {return;}
    }
}

void farmwatermarking(std::string path_in, std::string w_path_in, std::string path_out,bool compute_avg,int par_deg){
	path_in_par=path_in;
    //n_images_par = std::vector<int>(par_deg);
    proc_time_par = std::vector<float>(par_deg);
    save_time_par = std::vector<float>(par_deg);
    read_time_par = std::vector<float>(par_deg);

    //std::cout<<"farmwatermarking "<<std::endl;
    auto start_tc=std::chrono::high_resolution_clock::now();
    img_t watermark((w_path_in).c_str());
    std::vector<std::thread> tids;
    for(int i=0; i<par_deg; i++) {
        tids.push_back(std::thread(worker, i, watermark, path_out, compute_avg));
    }
    tids.push_back(std::thread(source, par_deg));
	    for(std::thread& t: tids){
	    	t.join();
    }
    auto elapsed_tc=std::chrono::high_resolution_clock::now()-start_tc;
	auto msec_tc=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_tc).count();
	//std::cout<<"processed "<<std::endl;
    //std::cout<<std::accumulate(n_images_par.begin(),n_images_par.end(),0)<<" ";
    std::cout<<std::accumulate(read_time_par.begin(),read_time_par.end(),0)/par_deg<<" "<<std::accumulate(proc_time_par.begin(),proc_time_par.end(),0)/par_deg<<" "<<std::accumulate(save_time_par.begin(),save_time_par.end(),0)/par_deg<<" ";
    std::cout<<msec_tc<<std::endl;
}