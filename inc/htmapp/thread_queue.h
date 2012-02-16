/*------------------------------------------------------------------
// Copyright (c) 1997 - 2011
// Robert Umbehant
// htmapp@wheresjames.com
// http://www.wheresjames.com
//
// Redistribution and use in source and binary forms, with or
// without modification, are permitted for commercial and
// non-commercial purposes, provided that the following
// conditions are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * The names of the developers or contributors may not be used to
//   endorse or promote products derived from this software without
//   specific prior written permission.
//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
//   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
//   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
//   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
//   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
//   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------*/

#pragma once

namespace tq
{

//------------------------------------------------------------------
// Messaging
//------------------------------------------------------------------

	/// Global Property bag
	extern t_pb g_htmapp_pb;

	/// Global Property bag lock
	extern CLock g_htmapp_pb_lock;

	/// Returns a reference to the thread pb
	t_pb& pb();

	/// Returns a reference to the thread pb
	CLock& lock();

	/// Sets the specified property value
	bool set( const str::t_string &sKey, const t_pb &pbValue, const str::t_string &sep = str::t_string() );

	/// Returns the specified property value
	t_pb get( const str::t_string &sKey, const str::t_string &sep = str::t_string() );

	/// Wait pool type
	typedef std::map< str::t_string, CEvent > t_waitpool;

	/// Wait pool object
	extern t_waitpool g_htmapp_wp;

	/// Wait for key to change
	/**
		Returns after uTimeout milliseconds or when ts::set() 
		is called with exact match for sKey.
	*/
	bool wait( const str::t_string &sKey, unsigned long uTimeout );

//------------------------------------------------------------------
// Thread pool
//------------------------------------------------------------------

	/// Thread function type
	typedef long (*t_tqfunc)( CThread*, void* );

	/// Worker thread type
	struct CWorkerThread : public CThread
	{	CWorkerThread();
		~CWorkerThread();
		void Run( t_tqfunc f, void *p, bool bStart );
		virtual long DoThread( void* x_pData ); 
		t_tqfunc m_f;
		void *m_p;
	};

	/// Thread pool class
	class CThreadPool : public CThread
	{
	public:

		struct SCmdInfo
		{
			// Constructors
			SCmdInfo() { f = 0; p = 0; }
			SCmdInfo( t_tqfunc x_f, void *x_p ) { f = x_f; p = x_p; }
			SCmdInfo( const SCmdInfo &r ) { f = r.f; p = r.p; }

			/// Function pointer
			t_tqfunc 	f;

			/// User data pointer
			void		*p;
		};

		/// Thread pool map type
		typedef std::map< long, CWorkerThread > t_threadpool;

		/// Thread command map type
		typedef std::map< long, SCmdInfo >		t_threadcmds;

	public:

		// Constructor
		CThreadPool();
		
		/// Destructor
		~CThreadPool();

		/// Adds a thread to the pool
		/**
			@return Thread id or less than zero if failure
		*/
		long start( t_tqfunc f, void *p = 0 );

		/// Stops the specified thread
		bool stop( long id );

	protected:

		/// Thread pool manager thread
		virtual long DoThread( void* x_pData );

		/// Called before thread exits
		virtual long EndThread( void* x_pData );

	private:

		/// Thread pool object
		t_threadpool m_tp;

		/// Thread pool commands
		t_threadcmds m_cmds;

		/// Command waiting event
		CEvent m_event;

		/// Command buffer lock
		CLock m_lock;

		/// Next thread id
		long m_id;

		/// Non-zero if thread pool is running
		bool m_enable;
	};

	/// Thread pool object
	extern CThreadPool g_htmapp_tp;

	/// Starts the specified thread
	/**
		@return Thread id or less than zero if failure
	*/
	long start( t_tqfunc f, void *p = 0 );

	/// Stops the specified thread
	bool stop( long id );

}; // namespace tq
