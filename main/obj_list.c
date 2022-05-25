 #include "obj_list.h"
/*********************************************************************** 
Function: decodes target list frame received from IMD-2000. 
Input arguments: 
- Frame array:  array with from IMD2000 received target list frame 
Output arguments: 
- targetList: struct with decoded target list 
Return value: 
- ErrorCode 
***********************************************************************/ 
IMD2000_Result_t IMD2000_decodeTargetFrame(unsigned char *frame_array,IMD2000_TargetList_t *targetList){ 
    uint16_t ui16_fc; 
    uint16_t ui16_nrOfTargets; 
    uint8_t *pData; 
    uint32_t ui32_tmp; 
    uint16_t ui16_tmp; 
    uint16_t ui16_targetCounter; 
 
    if(frame_array[0] == 0x68)       /* check SD2 Frame */ 
    { 
       return IMD2000_API_ERR_COMMAND_NO_VALID_FRAME_FOUND;    /* variable length frames not supported */ 
    } 
    else 
    { 
        ui16_fc = 3;  /* set function code bit for fixed length frames  */ 
    } 
 
    ui16_nrOfTargets = ((uint16_t)(frame_array[ui16_fc+1] & 0x00ff) << 8) + (uint16_t)(frame_array[ui16_fc+2] & 0x00ff); 
 
    if(ui16_nrOfTargets > IMD2000_MAX_TARGETS){ 
        return IMD2000_API_ERR_COMMAND_FAILURE; 
    } 
    targetList->ui16_nrOfTargets =          ui16_nrOfTargets; 
    targetList->ui16_targetListId =         ((uint16_t)(frame_array[ui16_fc+3] & 0x00ff) << 8) + (uint16_t)(frame_array[ui16_fc+4] & 0x00ff); 
    targetList->ui16_blockageDetection =    ((uint16_t)(frame_array[ui16_fc+5] & 0x00ff) << 8) + (uint16_t)(frame_array[ui16_fc+6] & 
0x00ff); 
    targetList->ui16_reserved =             ((uint16_t)(frame_array[ui16_fc+7] & 0x00ff) << 8) + (uint16_t)(frame_array[ui16_fc+8] & 0x00ff); 
 
    pData = &frame_array[ui16_fc+9]; 
    for(ui16_targetCounter = 0u; ui16_targetCounter < ui16_nrOfTargets; ui16_targetCounter++){ 
 
        ui32_tmp = (((*pData++)&0x000000ff) << 24); 
        ui32_tmp|=(((*pData++)&0x000000ff) << 16); 
        ui32_tmp|=(((*pData++)&0x000000ff) << 8); 
        ui32_tmp|=((*pData++)&0x000000ff); 
        targetList->target[ui16_targetCounter].f32_range_m = *(float32_t*)&ui32_tmp; 
 
        ui32_tmp = (((*pData++)&0x000000ff) << 24); 
        ui32_tmp|=(((*pData++)&0x000000ff) << 16); 
        ui32_tmp|=(((*pData++)&0x000000ff) << 8); 
        ui32_tmp|=((*pData++)&0x000000ff); 
        targetList->target[ui16_targetCounter].f32_velocity_mps = *(float32_t*)&ui32_tmp; 
 
        ui32_tmp = (((*pData++)&0x000000ff) << 24); 
        ui32_tmp|=(((*pData++)&0x000000ff) << 16); 
        ui32_tmp|=(((*pData++)&0x000000ff) << 8); 
        ui32_tmp|=((*pData++)&0x000000ff);
        targetList->target[ui16_targetCounter].f32_signal_dB = *(float32_t*)&ui32_tmp; 
 
        ui32_tmp = (((*pData++)&0x000000ff) << 24); 
        ui32_tmp|=(((*pData++)&0x000000ff) << 16); 
        ui32_tmp|=(((*pData++)&0x000000ff) << 8); 
        ui32_tmp|=((*pData++)&0x000000ff); 
        targetList->target[ui16_targetCounter].f32_estimatedTimeOfArrival_s = *(float32_t*)&ui32_tmp; 
    } 
 
    return IMD2000_API_ERR_OK; 
} 
 