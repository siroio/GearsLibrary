#pragma once

namespace Glib::Internal::Graphics::ID
{
    using UCHAR = unsigned char;

    /* ���_���\�[�XID */

    constexpr UCHAR CAMERA_VERTEX = 0;
    constexpr UCHAR IMAGE_VERTEX = CAMERA_VERTEX + 1;

    /* �p�C�v���C��ID */

    constexpr UCHAR CAMERA_PIPELINESTATE = 0;                                           /* �J���� */
    constexpr UCHAR SPRITE_PIPELINESTATE = CAMERA_PIPELINESTATE + 1;                    /* �X�v���C�g */
    constexpr UCHAR IMAGE_PIPELINESTATE = SPRITE_PIPELINESTATE + 1;                     /* UI */
    constexpr UCHAR LINE_PIPELINESTATE = IMAGE_PIPELINESTATE + 1;                       /* ���C�� */

    constexpr UCHAR MESH_PIPELINESTATE = LINE_PIPELINESTATE + 1;                        /* ���b�V�� */
    constexpr UCHAR MESH_SHADOW_PIPELINESTATE = MESH_PIPELINESTATE + 1;                 /* ���b�V�� */

    constexpr UCHAR SKINNED_MESH_PIPELINESTATE = MESH_SHADOW_PIPELINESTATE + 1;         /* �X�L�j���O���b�V��  */
    constexpr UCHAR SKINNED_MESH_SHADOW_PIPELINESTATE = SKINNED_MESH_PIPELINESTATE + 1; /* �X�L�j���O���b�V��  */

    /* �V�F�[�_�[ID */

    constexpr UCHAR SPRITE_SHADER = 0;
    constexpr UCHAR CAMERA_SHADER = SPRITE_SHADER + 1;
    constexpr UCHAR IMAGE_SHADER = CAMERA_SHADER + 1;

    /* �e�N�X�`��ID */

    constexpr UCHAR WHITE_TEXTURE = 0;
    constexpr UCHAR NORMAL_MAP_TEXTURE = WHITE_TEXTURE + 1;
}
