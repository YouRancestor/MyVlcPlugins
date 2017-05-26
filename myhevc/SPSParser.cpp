#include "stdlib.h"
#include "stdio.h"
#include "SPSParser.h"
#include <math.h>
#include <vector>


#define log2(x) ( (1/log(2)) * log( (x) ) )




SPSParser::SPSParser()
{
    m_h = h264_new();
    m_is265 = true;
    m_pParams = new vc_params_t;
    *m_pParams = {0};
}


SPSParser::~SPSParser()
{
    if(m_h)  h264_free(m_h);
    if(m_pParams) delete m_pParams;
}
void SPSParser::getSize(int& width, int& height)
{
    if(m_is265)
    {
        //TODO
        width = m_pParams->width;
        height = m_pParams->height;
    }
    else
    {
        width = (m_h->sps->pic_width_in_mbs_minus1 + 1) * 16;
        height = (m_h->sps->pic_height_in_map_units_minus1 + 1) * 16;
    }
}


SPSPARSESTATUS SPSParser::parseSPS(char* data, int size, bool is265)
{
    SPSPARSESTATUS ret;
    if(is265)
    {
        //TODO
        m_is265 = true;

        char *Sps = NULL;
        int i = 0;
        for(i = 0; i < size-4; i++ )
        {
            if( data[i]==0x00 && data[i+1]==0x00 && data[i+2]==0x00 && data[i+3]==0x01 && data[i+4]==0x42 )
            {
                Sps  =  &data[i+4];

                break;
            }
        }
        if( i == size )
        {
            return NO_SPS_FOUND;
        }

        if(!parseSequenceParameterSet( (unsigned char*)Sps, 41, *m_pParams ))
        {
            return BAD_DATA;
        }

        Sps = NULL;

        ret = PARSE_SUCCESS;
    }
    else
    {
        m_is265 = false;
        NAL* pNal = new NAL;
        pNal->length = size;
        pNal->bitstream = new unsigned char[size];
        memcpy(pNal->bitstream, (unsigned char*)data, size);

        ret = readSPS(pNal);
        if(pNal->bitstream)
            delete[] pNal->bitstream;
    }
    return ret;
}

SPSPARSESTATUS SPSParser::readSPS(NAL* pFirstNal)
{
    if(pFirstNal->length < 5)
    {
        printf("arguement of readSPS(NAL* pFirstNal) is bad\n");
        return NO_NAL_FOUND;
    }

    //1. seperate pFirstNal string into vector<NAL>
    std::vector<int> NAL_pos;
    unsigned int i = 0;
    while( i < pFirstNal->length - 3 )
    {
        // SPS, PPS, P, B, and the I frame
        if( pFirstNal->bitstream[i]==0x00 && pFirstNal->bitstream[i+1]==0x00 && pFirstNal->bitstream[i+2]==0x00 && pFirstNal->bitstream[i+3]==0x01 )
        {
            NAL_pos.push_back(i);
            i += 4;
        }
        // Other I frames
        else if( pFirstNal->bitstream[i]==0x00 && pFirstNal->bitstream[i+1]==0x00 && pFirstNal->bitstream[i+2]==0x01/* && File_nals[i+3]==0x65*/ )
        {
            NAL_pos.push_back(i);
            i += 3;
        }
        else
        {
            i++;
        }
    }
    // Add the end position
    NAL_pos.push_back( pFirstNal->length );

    std::vector<NAL> NAL_vec;
    for( i = 0; i != NAL_pos.size() - 1; ++i )
    {
        NAL nal;
        nal.length = NAL_pos[i+1] - NAL_pos[i];
        nal.bitstream = new unsigned char [nal.length];
        memcpy( nal.bitstream, pFirstNal->bitstream + NAL_pos[i], nal.length*sizeof(unsigned char) );
        //for( int j=0; j<nal.length; j++ ) printf( "%0x", nal.bitstream[j] ); printf( "\n" );
        NAL_vec.push_back( nal );
    }
    if(0 == NAL_vec.size())
    {
        printf("Seperate NAL_vec Failed : NAL_vec.size() == 0\n");
        return NO_NAL_FOUND;
    }

    //2. read each NAL until read SPS(meanwhile set pFirstNal's curPos and curLen)
    i = 0;
    unsigned int len_offset = 0;
    while(i != NAL_vec.size() && !read_nal_unit(NAL_vec.at(i).bitstream, NAL_vec.at(i).length))
    {
        len_offset += NAL_vec.at(i).length;
        ++i;
    }
    if(i != NAL_vec.size())
    {
        pFirstNal->curPos = pFirstNal->bitstream + len_offset;
        pFirstNal->curLen = pFirstNal->length - len_offset;
        return PARSE_SUCCESS;
    }
    else
    {
        return NO_SPS_FOUND;
    }
    //return ret;
    //return 0 : success
    //return 1 : not found , go on reading
    //return 2 : bad , termonate
}

h264_stream_t* SPSParser::h264_new()
{
    h264_stream_t* h = (h264_stream_t*)malloc(sizeof(h264_stream_t));
    h->nal = (nal_t*)malloc(sizeof(nal_t));
    h->sps = (sps_t*)malloc(sizeof(sps_t));
    //h->pps = (pps_t*)malloc(sizeof(pps_t));
    //h->aud = (aud_t*)malloc(sizeof(aud_t));
    //h->sh = (slice_header_t*)malloc(sizeof(slice_header_t));
    return h;
}


void SPSParser::h264_free(h264_stream_t* h)
{
    free(h->nal);
    free(h->sps);
    //free(h->pps);
    //free(h->aud);
    //free(h->sh);
    free(h);
}


// Sequence parameter set RBSP syntax
void SPSParser::read_seq_parameter_set_rbsp(h264_stream_t* h, bs_t* b)
{
    sps_t* sps = h->sps;

    int i;

    sps->profile_idc = bs_read_u8(b);
    sps->constraint_set0_flag = bs_read_u1(b);
    sps->constraint_set1_flag = bs_read_u1(b);
    sps->constraint_set2_flag = bs_read_u1(b);
    sps->constraint_set3_flag = bs_read_u1(b);
    sps->reserved_zero_4bits = bs_read_u(b,4);  /* all 0's */
    sps->level_idc = bs_read_u8(b);
    sps->seq_parameter_set_id = bs_read_ue(b);
    if( sps->profile_idc == 100 || sps->profile_idc == 110 ||
        sps->profile_idc == 122 || sps->profile_idc == 144 )
    {
        sps->chroma_format_idc = bs_read_ue(b);
        if( sps->chroma_format_idc == 3 )
        {
            sps->residual_colour_transform_flag = bs_read_u1(b);
        }
        sps->bit_depth_luma_minus8 = bs_read_ue(b);
        sps->bit_depth_chroma_minus8 = bs_read_ue(b);
        sps->qpprime_y_zero_transform_bypass_flag = bs_read_u1(b);
        sps->seq_scaling_matrix_present_flag = bs_read_u1(b);
        if( sps->seq_scaling_matrix_present_flag )
        {
            for( i = 0; i < 8; i++ )
            {
                sps->seq_scaling_list_present_flag[ i ] = bs_read_u1(b);
                if( sps->seq_scaling_list_present_flag[ i ] )
                {
                    if( i < 6 )
                    {
                        read_scaling_list( b, sps->ScalingList4x4[ i ], 16,
                                      sps->UseDefaultScalingMatrix4x4Flag[ i ]);
                    }
                    else
                    {
                        read_scaling_list( b, sps->ScalingList8x8[ i - 6 ], 64,
                                      sps->UseDefaultScalingMatrix8x8Flag[ i - 6 ] );
                    }
                }
            }
        }
    }
    sps->log2_max_frame_num_minus4 = bs_read_ue(b);
    sps->pic_order_cnt_type = bs_read_ue(b);
    if( sps->pic_order_cnt_type == 0 )
    {
        sps->log2_max_pic_order_cnt_lsb_minus4 = bs_read_ue(b);
    }
    else if( sps->pic_order_cnt_type == 1 )
    {
        sps->delta_pic_order_always_zero_flag = bs_read_u1(b);
        sps->offset_for_non_ref_pic = bs_read_se(b);
        sps->offset_for_top_to_bottom_field = bs_read_se(b);
        sps->num_ref_frames_in_pic_order_cnt_cycle = bs_read_ue(b);
        for( i = 0; i < sps->num_ref_frames_in_pic_order_cnt_cycle; i++ )
        {
            sps->offset_for_ref_frame[ i ] = bs_read_se(b);
        }
    }
    sps->num_ref_frames = bs_read_ue(b);
    sps->gaps_in_frame_num_value_allowed_flag = bs_read_u1(b);
    sps->pic_width_in_mbs_minus1 = bs_read_ue(b);
    sps->pic_height_in_map_units_minus1 = bs_read_ue(b);
//    sps->frame_mbs_only_flag = bs_read_u1(b);
//    if( !sps->frame_mbs_only_flag )
//    {
//        sps->mb_adaptive_frame_field_flag = bs_read_u1(b);
//    }
//    sps->direct_8x8_inference_flag = bs_read_u1(b);
//    sps->frame_cropping_flag = bs_read_u1(b);
//    if( sps->frame_cropping_flag )
//    {
//        sps->frame_crop_left_offset = bs_read_ue(b);
//        sps->frame_crop_right_offset = bs_read_ue(b);
//        sps->frame_crop_top_offset = bs_read_ue(b);
//        sps->frame_crop_bottom_offset = bs_read_ue(b);
//    }
//    sps->vui_parameters_present_flag = bs_read_u1(b);
//    if( sps->vui_parameters_present_flag )
//    {
//        read_vui_parameters(h, b);
//    }
//    read_rbsp_trailing_bits(h, b);
}


// Scaling list syntax
void SPSParser::read_scaling_list(bs_t* b, int* scalingList, int sizeOfScalingList, int useDefaultScalingMatrixFlag )
{
    int j;

    int lastScale = 8;
    int nextScale = 8;
    for( j = 0; j < sizeOfScalingList; j++ )
    {
        if( nextScale != 0 )
        {
            int delta_scale = bs_read_se(b);
            nextScale = ( lastScale + delta_scale + 256 ) % 256;
            useDefaultScalingMatrixFlag = ( j == 0 && nextScale == 0 );
        }
        scalingList[ j ] = ( nextScale == 0 ) ? lastScale : nextScale;
        lastScale = scalingList[ j ];
    }
}


bool SPSParser::read_nal_unit( unsigned char* p, unsigned int size)
{
    bool ret = false;
    unsigned char* buf = p;
    buf+=4;
    nal_t* nal = m_h->nal;

    bs_t* b = bs_new(buf, size);

    nal->forbidden_zero_bit = bs_read_f(b,1);
    nal->nal_ref_idc = bs_read_u(b,2);
    nal->nal_unit_type = bs_read_u(b,5);

    bs_free(b);

    unsigned char* rbsp_buf = (unsigned char*)malloc(size);
    int rbsp_size = 0;
    int nal_size = size;

    nal_to_rbsp(buf, &nal_size, rbsp_buf, &rbsp_size);

    b = bs_new(rbsp_buf, rbsp_size);

    if( nal->nal_unit_type == 7) { read_seq_parameter_set_rbsp(m_h, b); ret = true;}

//    if( nal->nal_unit_type == 0) { }                                 //  0    Unspecified
//    else if( nal->nal_unit_type == 1) { read_slice_layer_rbsp(h, b); }       //  1    Coded slice of a non-IDR picture
//    else if( nal->nal_unit_type == 2) {  }                           //  2    Coded slice data partition A
//    else if( nal->nal_unit_type == 3) {  }                           //  3    Coded slice data partition B
//    else if( nal->nal_unit_type == 4) {  }                           //  4    Coded slice data partition C
//    else if( nal->nal_unit_type == 5) { read_slice_layer_rbsp(h, b); }       //  5    Coded slice of an IDR picture
//    else if( nal->nal_unit_type == 6) { read_sei_rbsp(h, b); }         //  6    Supplemental enhancement information (SEI)
//    else if( nal->nal_unit_type == 7) { read_seq_parameter_set_rbsp(h, b); } //  7    Sequence parameter set
//    else if( nal->nal_unit_type == 8) { read_pic_parameter_set_rbsp(h, b); } //  8    Picture parameter set
//    else if( nal->nal_unit_type == 9) { read_access_unit_delimiter_rbsp(h, b); } //  9    Access unit delimiter
//    else if( nal->nal_unit_type == 10) { read_end_of_seq_rbsp(h, b); }       // 10    End of sequence
//    else if( nal->nal_unit_type == 11) { read_end_of_stream_rbsp(h, b); }    // 11    End of stream
//    else if( nal->nal_unit_type == 12) { /* read_filler_data_rbsp(h, b); */ }      // 12    Filler data
//    else if( nal->nal_unit_type == 13) { /* seq_parameter_set_extension_rbsp( ) */ } // 13    Sequence parameter set extension
//                                                                     //14..18 Reserved
//    else if( nal->nal_unit_type == 19) { read_slice_layer_rbsp(h, b); }      // 19    Coded slice of an auxiliary coded picture without partitioning
//                                                                      //20..23 Reserved
                                                                     //24..31 Unspecified

    bs_free(b); // TODO check for eof/read-beyond-end

    free(rbsp_buf);

    return ret;
}


/**
   Convert NAL data (Annex B format) to RBSP data.
   The size of rbsp_data must be the same as size of the nal data to guarantee the output will fit.
   If that is not true, output may be truncated.  If that is true, there is no possible error during this conversion.
   @param[in] nal_buf   the nal data
   @param[in] nal_size  pointer to the size of the nal data
   @param[in,out] rbsp_buf   allocated memory in which to put the rbsp data
   @param[in,out] rbsp_size  as input, pointer to the maximum size of the rbsp data; as output, filled in with the size actually written
 */


void SPSParser::nal_to_rbsp(unsigned char* nal_buf, int* nal_size, unsigned char* rbsp_buf, int* rbsp_size)
{
    // FIXME don't like using *nal_size etc syntax
    int i, j;

    i = 1; // NOTE omits first byte of NAL which contains nal_ref_idc and nal_unit_type, this is NOT part of the RBSP
    j = 0;
    while( i < *nal_size )
    {
        if( i + 2 < *nal_size &&
            nal_buf[i] == 0x00 && nal_buf[i+1] == 0x00 && nal_buf[i+2] == 0x03 ) // next_bits( 24 ) == 0x000003
        {
            rbsp_buf[ j   ] = nal_buf[ i   ];
            rbsp_buf[ j+1 ] = nal_buf[ i+1 ];
            // buf[ i+2 ] == 0x03  // skip emulation_prevention_three_byte equal to 0x03 // this is guaranteed from the above condition
            i += 3; j += 2;
        }
        else if (i + 2 < *nal_size &&
            nal_buf[i] == 0x00 && nal_buf[i+1] == 0x00 && nal_buf[i+2] == 0x01 ) // next_bits( 24 ) == 0x000001 // start of next nal, we're done
        {
            break;
        }
        else
        {
            rbsp_buf[ j ] = nal_buf[ i ];
            i += 1; j += 1;
        }
    }
    *nal_size = i;
    *rbsp_size = j;
}


/****** bitstream operations *******/

uint32_t SPSParser::bs_eof(bs_t* b) { if (b->p >= b->end) { return 1; } else { return 0; } }

bs_t* SPSParser::bs_new(unsigned char* buf, int size)
{
    bs_t* b = (bs_t*)malloc(sizeof(bs_t));
    bs_init(b, buf, size);
    return b;
}

void SPSParser::bs_free(bs_t* b)
{
    b->start = NULL;
    b->p = NULL;
    b->end = NULL;
    free(b);
}

void SPSParser::bs_init(bs_t* b, unsigned char* buf, int size)
{
    b->start = buf;
    b->p = buf;
    b->end = buf + size;
    b->bits_left = 8;
}

unsigned int SPSParser::bs_read_f(bs_t* b, int n) { return bs_read_u(b, n); }

unsigned int SPSParser::bs_read_u8(bs_t* b) { return bs_read_u(b, 8); }

unsigned int SPSParser::bs_read_ue(bs_t* b)
{
    int32_t r = 0;
    int i = 0;

    while( bs_read_u1(b) == 0 && i < 32 && !bs_eof(b) )
    {
        i++;
    }
    r = bs_read_u(b, i);
    r += (1 << i) - 1;
    return r;
}


uint32_t SPSParser::bs_read_u1(bs_t* b)
{
    uint32_t r = 0;
    if (bs_eof(b)) { return 0; }

    b->bits_left--;
    r = ((*(b->p)) >> b->bits_left) & 0x01;

    if (b->bits_left == 0) { b->p ++; b->bits_left = 8; }

    return r;
}

uint32_t SPSParser::bs_read_u(bs_t* b, int n)
{
    uint32_t r = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        r |= ( bs_read_u1(b) << ( n - i - 1 ) );
    }
    return r;
}


int32_t SPSParser::bs_read_se(bs_t* b)
{
    int32_t r = bs_read_ue(b);
    if (r & 0x01)
    {
        r = (r+1)/2;
    }
    else
    {
        r = -(r/2);
    }
    return r;
}

bool  SPSParser::parseSequenceParameterSet(unsigned char* data,int size, vc_params_t& params)
{
    if (size < 20)
    {
        return false;
    }

    NALBitstream bs(data, size);

    // seq_parameter_set_rbsp()
    bs.GetWord(4);// sps_video_parameter_set_id
    int sps_max_sub_layers_minus1 = bs.GetWord(3); // "The value of sps_max_sub_layers_minus1 shall be in the range of 0 to 6, inclusive."
    if (sps_max_sub_layers_minus1 > 6)
    {
        return false;
    }
    bs.GetWord(1);// sps_temporal_id_nesting_flag
    // profile_tier_level( sps_max_sub_layers_minus1 )
    {
        bs.GetWord(2);// general_profile_space
        bs.GetWord(1);// general_tier_flag
        params.profile = bs.GetWord(5);// general_profile_idc
        bs.GetWord(32);// general_profile_compatibility_flag[32]
        bs.GetWord(1);// general_progressive_source_flag
        bs.GetWord(1);// general_interlaced_source_flag
        bs.GetWord(1);// general_non_packed_constraint_flag
        bs.GetWord(1);// general_frame_only_constraint_flag
        bs.GetWord(44);// general_reserved_zero_44bits
        params.level   = bs.GetWord(8);// general_level_idc
        unsigned char sub_layer_profile_present_flag[6] = {0};
        unsigned char sub_layer_level_present_flag[6]   = {0};

        for (int i = 0; i < sps_max_sub_layers_minus1; i++)
        {
            sub_layer_profile_present_flag[i]= bs.GetWord(1);
            sub_layer_level_present_flag[i]= bs.GetWord(1);
        }

        if (sps_max_sub_layers_minus1 > 0)
        {
            for (int i = sps_max_sub_layers_minus1; i < 8; i++)
            {
                unsigned char reserved_zero_2bits = bs.GetWord(2);
            }
        }

        for (int i = 0; i < sps_max_sub_layers_minus1; i++)
        {
            if (sub_layer_profile_present_flag[i])
            {
                bs.GetWord(2);// sub_layer_profile_space[i]
                bs.GetWord(1);// sub_layer_tier_flag[i]
                bs.GetWord(5);// sub_layer_profile_idc[i]
                bs.GetWord(32);// sub_layer_profile_compatibility_flag[i][32]
                bs.GetWord(1);// sub_layer_progressive_source_flag[i]
                bs.GetWord(1);// sub_layer_interlaced_source_flag[i]
                bs.GetWord(1);// sub_layer_non_packed_constraint_flag[i]
                bs.GetWord(1);// sub_layer_frame_only_constraint_flag[i]
                bs.GetWord(44);// sub_layer_reserved_zero_44bits[i]
            }
            if (sub_layer_level_present_flag[i])
            {
                bs.GetWord(8);// sub_layer_level_idc[i]
            }
        }
    }

    unsigned int sps_seq_parameter_set_id= bs.GetUE(); // "The  value  of sps_seq_parameter_set_id shall be in the range of 0 to 15, inclusive."
    if (sps_seq_parameter_set_id > 15)
    {
        return false;
    }

    unsigned int chroma_format_idc = bs.GetUE(); // "The value of chroma_format_idc shall be in the range of 0 to 3, inclusive."
    if (sps_seq_parameter_set_id > 3)
    {
        return false;
    }

    if (chroma_format_idc == 3)
    {
        bs.GetWord(1);// separate_colour_plane_flag
    }

    params.width = bs.GetUE(); // pic_width_in_luma_samples
    params.height = bs.GetUE(); // pic_height_in_luma_samples
    if (bs.GetWord(1))
    {// conformance_window_flag
        bs.GetUE(); // conf_win_left_offset
        bs.GetUE(); // conf_win_right_offset
        bs.GetUE(); // conf_win_top_offset
        bs.GetUE(); // conf_win_bottom_offset
    }

    unsigned int bit_depth_luma_minus8= bs.GetUE();
    unsigned int bit_depth_chroma_minus8= bs.GetUE();
    if (bit_depth_luma_minus8 != bit_depth_chroma_minus8)
    {
        return false;
    }

    return true;
}



