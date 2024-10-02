__constant float gravity = 9.81 ;

float calculate_length( float2 a )
{
    return sqrt(dot(a, a)) ;
}

float dispersion_relation(float2 wavevector, float time)
{
    return sqrt(gravity * calculate_length(wavevector)) ;
}

__kernel void compute_FourierAmplitudes( __global float4 *fourier_grid, __global float2 *wavevectors, __global float4 *fourier_amplitudes, float time, int N )
{
    uint i = get_global_id(0) ;
    uint j = get_global_id(1) ;

    if( (i<N) && (j<N) ){
        float phase = dispersion_relation(wavevectors[i * N + j], time) * time ;
        float r1 = fourier_grid[i * N + j].x + fourier_grid[i * N + j].z ;
        float i1 = fourier_grid[i * N + j].y + fourier_grid[i * N + j].w ;
        float r2 = fourier_grid[i * N + j].x - fourier_grid[i * N + j].z ;
        float i2 = fourier_grid[i * N + j].y - fourier_grid[i * N + j].w ;
        float kx = wavevectors[i * N + j].x ;
        float ky = wavevectors[i * N + j].y ;
        fourier_amplitudes[i * N + j].x = (r1 * cos(phase)) - (i1 * sin(phase)) ;
        fourier_amplitudes[i * N + j].y = (r2 * sin(phase)) + (i2 * cos(phase)) ;
        fourier_amplitudes[i * N + j].z = -kx * i1 ;
        fourier_amplitudes[i * N + j].w = -ky * r1 ;
        
        if ((i == 0 && j == 0) || (i == N/2 && j == N/2))
            fourier_amplitudes[i * N + j].y = 0.0f;
    }
}

__kernel void inverse_Fourier_Transform( __global float4 *fourier_amplitudes, write_only image2d_t height_map, __global float2 *wavevectors, float time, int N, float L )
{
    uint i = get_global_id(0) ;
    uint j = get_global_id(1) ;
    
    if( (i<N) && (j<N) ){
        float4 sum = (float4)(0.0f, 0.0f, 0.0f, 0.0f) ;
        float2 spatial_position = (float2)( ((i-N/2.0f)*(L/N)), ((j-N/2.0f)*(L/N)) ) ;
        for( int m = 0 ; m < N ; m++ ){
            for ( int n = 0 ; n < N ; n++ ){
                float phase = dot( wavevectors[(m*N)+n], spatial_position ) ;
                float2 exponential = (float2)( cos(phase), sin(phase) ) ;
                sum.x += (fourier_amplitudes[(m*N)+n].x * exponential.x) - (fourier_amplitudes[(m*N)+n].y * exponential.y) ;
                sum.y += (fourier_amplitudes[(m*N)+n].x * exponential.y) + (fourier_amplitudes[(m*N)+n].y * exponential.x) ;
                sum.z += (fourier_amplitudes[(m*N)+n].z * exponential.x) - (fourier_amplitudes[(m*N)+n].w * exponential.y) ;
                sum.w += (fourier_amplitudes[(m*N)+n].z * exponential.y) + (fourier_amplitudes[(m*N)+n].w * exponential.x) ;
            }
        }
        sum = sum / (float)(N*N) ;
        float4 data = (float4)( sum.x, sum.z, sum.w, 1.0f ) ;
        write_imagef(height_map, (int2)(i, j), data);
    }
}
