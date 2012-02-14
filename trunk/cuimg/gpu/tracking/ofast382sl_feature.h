#ifndef CUIMG_OFAST382SL_FEATURE_H_
# define CUIMG_OFAST382SL_FEATURE_H_

# include <cuimg/gpu/image2d.h>
# include <cuimg/gpu/kernel_image2d.h>
# include <cuimg/point2d.h>
# include <cuimg/obox2d.h>
# include <cuimg/improved_builtin.h>

namespace cuimg
{

  struct dofast382sl
  {
    __host__ __device__ dofast382sl() : o1(0), o2(0) {}

    __host__ __device__ dofast382sl(const dofast382sl& o)
    {
      o1 = o.o1;
      o2 = o.o2;
    }

    __host__ __device__
    dofast382sl& operator=(const dofast382sl& o)
    {
      o1 = o.o1;
      o2 = o.o2;
      return *this;
    }

    char o1, o2;
  };

  struct dofast382sl_state
  {
    enum { size = 4 };
    unsigned char state[size];
    char o1, o2;
    char2 padding__;
  };

  class kernel_ofast382sl_feature;

  class ofast382sl_feature
  {
  public:
    typedef dofast382sl_state feature_t;
    typedef obox2d<point2d<int> > domain_t;

    typedef kernel_ofast382sl_feature kernel_type;

    inline ofast382sl_feature(const domain_t& d);

    inline void update(const image2d<i_float4>& in);
    inline void update(const image2d<i_float1>& in);

    inline const domain_t& domain() const;

    inline image2d<dofast382sl>& previous_frame();
    inline image2d<dofast382sl>& current_frame();
    inline image2d<i_float1>& pertinence();

    inline image2d<i_float1>& blurred_s2();
    inline image2d<i_float1>& blurred_s1();

    const image2d<i_float4>& feature_color() const;

    void display() const;

  private:
    inline void swap_buffers();


    image2d<i_float1> gl_frame_;
    image2d<i_float1> blurred_s1_;
    image2d<i_float1> blurred_s2_;
    image2d<i_float1> tmp_;

    image2d<i_float1> pertinence_;
    image2d<i_float1> pertinence2_;

    image2d<dofast382sl> f1_;
    image2d<dofast382sl> f2_;

    image2d<dofast382sl>* f_prev_;
    image2d<dofast382sl>* f_;

    image2d<i_float4> ofast382sl_color_;

    image2d<i_float4> color_blurred_;
    image2d<i_float4> color_tmp_;

    float grad_thresh;

    cudaStream_t cuda_stream_;
  };

  class kernel_ofast382sl_feature
  {
  public:
    typedef dofast382sl_state feature_t;

    inline kernel_ofast382sl_feature(ofast382sl_feature& f);


    inline
    __device__ float distance(const point2d<int>& p_prev,
                              const point2d<int>& p_cur);

    inline
    __device__ float distance_linear(const dofast382sl_state& desc,
                                     const point2d<int>& n);

    inline
    __device__ float distance_linear(const dofast382sl& a, const dofast382sl& b);
    inline
    __device__ float distance_linear_s2(const dofast382sl& a, const dofast382sl& b);
    inline
    __device__ float distance(const dofast382sl& a, const dofast382sl& b);
    inline
    __device__ float distance_s2(const dofast382sl& a, const dofast382sl& b);

    __device__ inline
    dofast382sl_state weighted_mean(const dofast382sl_state& a, float aw, const point2d<int>& b, float bw);

    inline __device__ dofast382sl_state
    new_state(const point2d<int>& n);

    /* __device__ inline */
    /* kernel_image2d<dofast382sl>& previous_frame(); */
    __device__ inline
    kernel_image2d<dofast382sl>& current_frame();
    __device__ inline
    kernel_image2d<i_float1>& pertinence();


  private:
    kernel_image2d<i_float1> pertinence_;
    //kernel_image2d<dofast382sl> f_prev_;
    kernel_image2d<dofast382sl> f_;
    kernel_image2d<i_float1> blurred_s1_;
    kernel_image2d<i_float1> blurred_s2_;
  };

}

# include <cuimg/gpu/tracking/ofast382sl_feature.hpp>

#endif // ! CUIMG_OFAST382SL_FEATURE_H_
