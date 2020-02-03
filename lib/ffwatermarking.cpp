#include <ff/farm.hpp>

using namespace ff;
using img_t = cimg_library::CImg<int>;

std::vector<float> read_time_ff;
std::vector<float> proc_time_ff;
std::vector<float> save_time_ff;
//std::vector<int> n_images_ff;
std::string path_in_ff;


struct source_node: ff_node_t<std::string> {

	source_node(int par_deg):
	par_deg(par_deg){};

	std::string* svc(std::string *in) {
		struct dirent *photoDirent;
		DIR *photoDir;
		photoDir = opendir (path_in_ff.c_str());

		if (photoDir == NULL) {
			std::cout<<"Cannot open directory "<<path_in_ff<<std::endl;
			return EOS;
		}

		while ((photoDirent = readdir(photoDir)) != NULL) {
			if (strcmp(photoDirent->d_name, ".") != 0 && strcmp(photoDirent->d_name, "..") != 0 ) 
			{
				std::string *file_name = new std::string(photoDirent->d_name);
				ff_send_out(file_name);
			}
		}
		closedir(photoDir);
		return EOS;
	}

	int par_deg;

};
struct worker_node: ff_node_t<std::string> {
	worker_node(int i, img_t watermark, std::string path_out, bool compute_avg):
	i(i), watermark(watermark), path_out(path_out), compute_avg(compute_avg){};

	std::string* svc(std::string *task) {
		std::string &file_name = *task;
		auto start_r=std::chrono::high_resolution_clock::now();
		img_t img((path_in_ff+file_name).c_str());
		auto elapsed_r=std::chrono::high_resolution_clock::now()-start_r;
        auto msec_r=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_r).count();
		read_time_ff[i]+=msec_r;

		auto start_w=std::chrono::high_resolution_clock::now();
		apply_watermark(img, watermark, compute_avg);
		auto elapsed_w=std::chrono::high_resolution_clock::now()-start_w;
        auto msec_w=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_w).count();
        proc_time_ff[i]+=msec_w;

		//n_images_ff[i]++;
		try{
			auto start_s=std::chrono::high_resolution_clock::now();
			img.save_jpeg((path_out+file_name).c_str(),100);
			auto elapsed_s=std::chrono::high_resolution_clock::now()-start_s;
        	auto msec_s=std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_s).count();
        	save_time_ff[i]+=msec_s;

		}catch(cimg_library::CImgIOException ex){
                std::cerr<<"Error occurred while saving "<<((path_out+file_name).c_str())<<std::endl;
            }
		delete task;
        return GO_ON;
	}
	int i;
	img_t watermark;
	std::string path_out;
	bool compute_avg;
};

int ffwatermarking(std::string path_in, std::string w_path_in, std::string path_out, bool compute_avg, int par_deg){
	path_in_ff=path_in;
    //n_images_ff = std::vector<int>(par_deg);
    proc_time_ff = std::vector<float>(par_deg);
    save_time_ff = std::vector<float>(par_deg);
    read_time_ff = std::vector<float>(par_deg);

	//std::cout<<"ffwatermarking "<<std::endl;
	img_t watermark(w_path_in.c_str());
	source_node src_node(par_deg);
	std::vector<std::unique_ptr<ff_node> > W;
	for(size_t i=0;i<par_deg;++i){ 
		W.push_back(make_unique<worker_node>(i,watermark,path_out,compute_avg));
	}

	ff_Farm<std::string> farm(std::move(W));
	farm.add_emitter(src_node);
	farm.remove_collector();
	//farm.set_scheduling_ondemand();
	ffTime(START_TIME);
	if (farm.run_and_wait_end()<0) {
		error("Running farm ");
		return -1;
	}

	ffTime(STOP_TIME);
	//std::cout<<"processed "<<std::endl;
    //std::cout<<std::accumulate(n_images_ff.begin(),n_images_ff.end(),0)<<" ";
    std::cout<<std::accumulate(read_time_ff.begin(),read_time_ff.end(),0)/par_deg<<" "<<std::accumulate(proc_time_ff.begin(),proc_time_ff.end(),0)/par_deg<<" "<<std::accumulate(save_time_ff.begin(),save_time_ff.end(),0)/par_deg<<" ";
	std::cout << ffTime(GET_TIME) <<std::endl;
	//farm.ffStats(std::cout);
	return 0;  
}