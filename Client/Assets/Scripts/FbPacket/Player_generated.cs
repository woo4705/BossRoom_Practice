// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace FbPacket
{

using global::System;
using global::System.Collections.Generic;
using global::Google.FlatBuffers;

public enum FbClassType : sbyte
{
  None = 0,
  Tank = 1,
  Archer = 2,
  Mage = 3,
  Rogue = 4,
};

public struct FbVec3 : IFlatbufferObject
{
  private Struct __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public void __init(int _i, ByteBuffer _bb) { __p = new Struct(_i, _bb); }
  public FbVec3 __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public float X { get { return __p.bb.GetFloat(__p.bb_pos + 0); } }
  public float Y { get { return __p.bb.GetFloat(__p.bb_pos + 4); } }
  public float Z { get { return __p.bb.GetFloat(__p.bb_pos + 8); } }

  public static Offset<FbPacket.FbVec3> CreateFbVec3(FlatBufferBuilder builder, float X, float Y, float Z) {
    builder.Prep(4, 12);
    builder.PutFloat(Z);
    builder.PutFloat(Y);
    builder.PutFloat(X);
    return new Offset<FbPacket.FbVec3>(builder.Offset);
  }
}

public struct FbQuat : IFlatbufferObject
{
  private Struct __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public void __init(int _i, ByteBuffer _bb) { __p = new Struct(_i, _bb); }
  public FbQuat __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public float X { get { return __p.bb.GetFloat(__p.bb_pos + 0); } }
  public float Y { get { return __p.bb.GetFloat(__p.bb_pos + 4); } }
  public float Z { get { return __p.bb.GetFloat(__p.bb_pos + 8); } }
  public float W { get { return __p.bb.GetFloat(__p.bb_pos + 12); } }

  public static Offset<FbPacket.FbQuat> CreateFbQuat(FlatBufferBuilder builder, float X, float Y, float Z, float W) {
    builder.Prep(4, 16);
    builder.PutFloat(W);
    builder.PutFloat(Z);
    builder.PutFloat(Y);
    builder.PutFloat(X);
    return new Offset<FbPacket.FbQuat>(builder.Offset);
  }
}

public struct FbPlayer : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_23_3_3(); }
  public static FbPlayer GetRootAsFbPlayer(ByteBuffer _bb) { return GetRootAsFbPlayer(_bb, new FbPlayer()); }
  public static FbPlayer GetRootAsFbPlayer(ByteBuffer _bb, FbPlayer obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public FbPlayer __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long Id { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetLong(o + __p.bb_pos) : (long)0; } }
  public string NickName { get { int o = __p.__offset(6); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNickNameBytes() { return __p.__vector_as_span<byte>(6, 1); }
#else
  public ArraySegment<byte>? GetNickNameBytes() { return __p.__vector_as_arraysegment(6); }
#endif
  public byte[] GetNickNameArray() { return __p.__vector_as_array<byte>(6); }
  public FbPacket.FbVec3? Position { get { int o = __p.__offset(8); return o != 0 ? (FbPacket.FbVec3?)(new FbPacket.FbVec3()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public FbPacket.FbQuat? Rotation { get { int o = __p.__offset(10); return o != 0 ? (FbPacket.FbQuat?)(new FbPacket.FbQuat()).__assign(o + __p.bb_pos, __p.bb) : null; } }
  public short Hp { get { int o = __p.__offset(12); return o != 0 ? __p.bb.GetShort(o + __p.bb_pos) : (short)0; } }
  public string Name { get { int o = __p.__offset(14); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNameBytes() { return __p.__vector_as_span<byte>(14, 1); }
#else
  public ArraySegment<byte>? GetNameBytes() { return __p.__vector_as_arraysegment(14); }
#endif
  public byte[] GetNameArray() { return __p.__vector_as_array<byte>(14); }
  public FbPacket.FbClassType Class { get { int o = __p.__offset(16); return o != 0 ? (FbPacket.FbClassType)__p.bb.GetSbyte(o + __p.bb_pos) : FbPacket.FbClassType.None; } }

  public static void StartFbPlayer(FlatBufferBuilder builder) { builder.StartTable(7); }
  public static void AddId(FlatBufferBuilder builder, long id) { builder.AddLong(0, id, 0); }
  public static void AddNickName(FlatBufferBuilder builder, StringOffset nickNameOffset) { builder.AddOffset(1, nickNameOffset.Value, 0); }
  public static void AddPosition(FlatBufferBuilder builder, Offset<FbPacket.FbVec3> positionOffset) { builder.AddStruct(2, positionOffset.Value, 0); }
  public static void AddRotation(FlatBufferBuilder builder, Offset<FbPacket.FbQuat> rotationOffset) { builder.AddStruct(3, rotationOffset.Value, 0); }
  public static void AddHp(FlatBufferBuilder builder, short hp) { builder.AddShort(4, hp, 0); }
  public static void AddName(FlatBufferBuilder builder, StringOffset nameOffset) { builder.AddOffset(5, nameOffset.Value, 0); }
  public static void AddClass(FlatBufferBuilder builder, FbPacket.FbClassType @class) { builder.AddSbyte(6, (sbyte)@class, 0); }
  public static Offset<FbPacket.FbPlayer> EndFbPlayer(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<FbPacket.FbPlayer>(o);
  }
  public static void FinishFbPlayerBuffer(FlatBufferBuilder builder, Offset<FbPacket.FbPlayer> offset) { builder.Finish(offset.Value); }
  public static void FinishSizePrefixedFbPlayerBuffer(FlatBufferBuilder builder, Offset<FbPacket.FbPlayer> offset) { builder.FinishSizePrefixed(offset.Value); }
}


}
