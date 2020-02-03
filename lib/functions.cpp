using img_t = cimg_library::CImg<int>;

void apply_watermark(img_t &img, img_t &watermark, bool compute_avg){
    int width = img.width();
    int height = img.height();
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            if(watermark(x,y)<50){ 
                if(!compute_avg){
                    for(int ch = 0; ch < img.spectrum(); ch++){
                        img(x,y,0,ch) = 0;
                    }
                }else{
                    int avg = ((int)img(x,y,0,0) + (int)img(x,y,0,1) + (int) img(x,y,0,2) + 0)/(img.spectrum()+1);
                    for(int ch = 0; ch < img.spectrum(); ch++){
                        img(x,y,0,ch) = avg;
                    }
                }
            }
        }
    }
    return;
}