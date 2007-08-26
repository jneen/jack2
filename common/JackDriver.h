/*
Copyright (C) 2001 Paul Davis 
Copyright (C) 2004-2006 Grame

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef __JackDriver__
#define __JackDriver__

#include "types.h"
#include "JackClientInterface.h"
#include "JackConstants.h"
#include <list>

namespace Jack
{

class JackEngine;
class JackGraphManager;
class JackSynchro;
struct JackEngineControl;
struct JackClientControl;

/*!
\brief The base interface for drivers.
*/

class EXPORT JackDriverInterface
{

    public:

        JackDriverInterface()
        {}
        virtual ~JackDriverInterface()
        {}

        virtual int Open() = 0;

        virtual int Open(jack_nframes_t nframes,
                         jack_nframes_t samplerate,
                         int capturing,
                         int playing,
                         int inchannels,
                         int outchannels,
                         bool monitor,
                         const char* capture_driver_name,
                         const char* playback_driver_name,
                         jack_nframes_t capture_latency,
                         jack_nframes_t playback_latency) = 0;

        virtual int Attach() = 0;
        virtual int Detach() = 0;
	
        virtual int Read() = 0;
        virtual int Write() = 0;
		
        virtual int Start() = 0;
        virtual int Stop() = 0;
        virtual int SetBufferSize(jack_nframes_t buffer_size) = 0;

        virtual int Process() = 0;

        virtual void SetMaster(bool onoff) = 0;
        virtual bool GetMaster() = 0;
        virtual void AddSlave(JackDriverInterface* slave) = 0;
        virtual void RemoveSlave(JackDriverInterface* slave) = 0;
        virtual int ProcessSlaves() = 0;

        virtual bool IsRealTime() = 0;

        virtual void PrintState() = 0;
};

/*!
\brief The base interface for drivers clients.
*/

class EXPORT JackDriverClientInterface : public JackDriverInterface, public JackClientInterface
{};

/*!
\brief The base class for drivers clients.
*/

class EXPORT JackDriverClient : public JackDriverClientInterface
{
    private:

        std::list<JackDriverInterface*> fSlaveList;

    protected:

        bool fIsMaster;

    public:

        virtual void SetMaster(bool onoff);
        virtual bool GetMaster();
        virtual void AddSlave(JackDriverInterface* slave);
        virtual void RemoveSlave(JackDriverInterface* slave);
        virtual int ProcessSlaves();
};

/*!
\brief The base class for drivers.
*/

class EXPORT JackDriver : public JackDriverClient
{

    protected:

        char fCaptureDriverName[JACK_CLIENT_NAME_SIZE];
        char fPlaybackDriverName[JACK_CLIENT_NAME_SIZE];
        jack_nframes_t fCaptureLatency;
        jack_nframes_t fPlaybackLatency;
        jack_time_t fLastWaitUst;
        JackEngine*	fEngine;
        JackGraphManager* fGraphManager;
        JackSynchro** fSynchroTable;
        JackEngineControl* fEngineControl;
        JackClientControl* fClientControl;

        JackClientControl* GetClientControl() const;

    public:

        JackDriver(const char* name, JackEngine* engine, JackSynchro** table);
        JackDriver();
        virtual ~JackDriver();

        virtual int Open();

        virtual int Open(jack_nframes_t nframes,
                         jack_nframes_t samplerate,
                         int capturing,
                         int playing,
                         int inchannels,
                         int outchannels,
                         bool monitor,
                         const char* capture_driver_name,
                         const char* playback_driver_name,
                         jack_nframes_t capture_latency,
                         jack_nframes_t playback_latency);

        virtual int Close();

        virtual int Process()
        {
            return 0;
        }

        virtual int Attach()
        {
            return 0;
        }
        virtual int Detach()
        {
            return 0;
        }

        virtual int Read()
        {
            return 0;
        }
        virtual int Write()
        {
            return 0;
        }

        virtual int Start()
        {
            return 0;
        }
        virtual int Stop()
        {
            return 0;
        }

        virtual int SetBufferSize(jack_nframes_t buffer_size)
        {
            return 0;
        }

        void NotifyXRun(jack_time_t callback_usecs); // XRun notification sent by the driver

        virtual bool IsRealTime();

        virtual void PrintState()
        {}

        int ClientNotify(int refnum, const char* name, int notify, int sync, int value)
        {
            return 0;
        }

};

} // end of namespace

#endif
