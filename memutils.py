import ctypes


def get_pyobject_from_pointer(memory):
    return ctypes.cast(memory, ctypes.POINTER(ctypes.py_object)).contents.value


def get_pyobject_from_memory(memory):
    return ctypes.cast(memory, ctypes.py_object).value


def memory_dump(start_address, end_address):
    size = end_address - start_address
    buffer = ctypes.create_string_buffer(size)
    ctypes.memmove(buffer, start_address, size)
    hex_chunks = [buffer.raw[i : i + 8].hex() for i in range(0, size, 8)]
    return hex_chunks
